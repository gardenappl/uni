import Control.Concurrent
import Control.DeepSeq (($!!))
import Data.Bifunctor
import Data.Binary.Get
import Data.Binary.Put
import Data.Bits
import qualified Data.ByteString.Lazy as B
import Data.Char
import Data.Function (on)
import Data.List
import qualified Data.Map as Map
import Data.Maybe
import Data.Word
import System.Environment
import System.IO

type Symbol = Word8

type Weight = Int

data Node
  = Leaf 
      { sym :: Symbol
      , weight :: Weight
      }
  | Interior
      { l :: Node,
        r :: Node,
        syms :: [Symbol],
        weight :: Weight
      }
  deriving (Show)

getSymbols :: Node -> [Symbol]
getSymbols (Leaf s _) = [s]
getSymbols (Interior _ _ syms _) = syms

getWeight :: Node -> Weight
getWeight (Leaf _ w) = w
getWeight (Interior _ _ _ w) = w

orderedInsert :: Node -> [Node] -> [Node]
orderedInsert n [] = [n]
orderedInsert n nodes = insertBy (compare `on` weight) n nodes

-- Input: leaf nodes sorted by weight in ascending order
-- Output: Huffman tree
makeTree :: [Node] -> Node
makeTree [n] = n
makeTree (first : second : rest) =
  let n = Interior first second (getSymbols first ++ getSymbols second) (getWeight first + getWeight second)
   in makeTree (orderedInsert n rest)

-- Decode bool list based on tree
decode' :: Node -> Node -> [Bool] -> [Symbol]
decode' codeTree (Leaf s w) bits =
  s : decode' codeTree codeTree bits
decode' codeTree (Interior l _ _ _) (False : bits) =
  decode' codeTree l bits
decode' codeTree (Interior _ r _ _) (True : bits) =
  decode' codeTree r bits
decode' _ _ [] = []

decode :: Node -> [Bool] -> B.ByteString
decode tree bits = B.pack (decode' tree tree bits)

-- Encode symbols into bool list based on tree
encode' :: Node -> Node -> [Symbol] -> [Bool]
encode' _ _ [] = []
encode' codeTree (Leaf s w) (sym : symbols) =
  encode' codeTree codeTree symbols
encode' codeTree (Interior l r _ _) (sym : symbols)
  | sym `elem` getSymbols l = False : encode' codeTree l (sym : symbols)
  | otherwise               = True : encode' codeTree r (sym : symbols)

encode :: Node -> B.ByteString -> [Bool]
encode tree str = encode' tree tree (B.unpack str)



data FileArchivalMeta = FileArchivalMeta { fileName :: String, weights :: [Node], tree :: Node, encodedLength :: Int }

-- Get all symbols in file and their weights, as list
scanFile :: FilePath -> IO [(Word8, Int)]
scanFile path = withBinaryFile path ReadMode (\handle -> do
  contents <- B.hGetContents handle
  -- lazy IO, must force reading the whole file with $!!
  return $!! Map.toList (Map.fromListWith (+) (map (\x -> (x, 1)) (B.unpack contents))))

toNodes :: [(Word8, Int)] -> [Node]
toNodes symWeights = sortBy (compare `on` weight) (map (uncurry Leaf) symWeights)


-- First pass for reading file
doFirstPass :: FilePath -> IO FileArchivalMeta
doFirstPass filePath = do
  symWeights <- scanFile filePath
  -- putStrLn filePath
  let nodes = toNodes symWeights
  -- print nodes
  let tree = makeTree nodes
  -- print (getAllEncodings tree)
  let treeDepths = getTreeDepths tree

  let encodedLength = getEncodedLength treeDepths symWeights
  return (FileArchivalMeta filePath nodes tree encodedLength)

-- Second pass: read file, encode and append to archive
doSecondPass :: Handle -> FileArchivalMeta -> IO ()
doSecondPass archiveHandle (FileArchivalMeta fileName weights tree encodedLength) = do
  putStr "Writing: "
  putStrLn fileName
  encodeName archiveHandle fileName
  encodeNodes archiveHandle weights
  hPutWord32le archiveHandle encodedLength

  encoded <- withBinaryFile fileName ReadMode (\handle -> do
    contents <- B.hGetContents handle
    return $!! encode tree contents)
  B.hPut archiveHandle (B.pack (boolsToWords encoded))
  

encodeFilesSeq' :: Handle -> [FilePath] -> IO ()
encodeFilesSeq' _ [] = return ()
encodeFilesSeq' handle (filePath:files) = do
  putStr "Scanning "
  putStrLn filePath
  meta@(FileArchivalMeta name weights tree length) <- doFirstPass filePath

  doSecondPass handle meta
  encodeFilesSeq' handle files

encodeFilesSeq :: FilePath -> [FilePath] -> IO ()
encodeFilesSeq archivePath files = withBinaryFile archivePath WriteMode (\handle ->
  encodeFilesSeq' handle files)


encodeFilesPar :: FilePath -> [FilePath] -> IO ()
encodeFilesPar archivePath files = withBinaryFile archivePath WriteMode (\handle -> do
  chan <- newChan

  -- do first passes for each file
  mapM_ (\fileName -> forkIO (do 
    fileArchivalMeta@(FileArchivalMeta name _ _ _) <- doFirstPass fileName
    writeChan chan fileArchivalMeta
    putStr "Scanned "
    putStrLn name
    )) files

  chanContents <- getChanContents chan
  let fileMetas = take (length files) chanContents

  mapM_ (\meta@(FileArchivalMeta name _ _ _) -> do
    doSecondPass handle meta
    putStr "Wrote "
    putStrLn name) fileMetas)
  


decodeFiles' :: [Word8] -> IO ()
decodeFiles' [] = return ()
decodeFiles' words = do
  let (name, nodeBytes) = decodeName words
  putStr "Decoding: "
  putStrLn name
  let (nodes, fileBytes) = decodeNodes nodeBytes
  let tree = makeTree nodes
  -- print (getAllEncodings tree)
  let (file, rest) = decodeSingleFile fileBytes tree
  B.writeFile name file
  decodeFiles' rest


decodeFiles :: FilePath -> IO ()
decodeFiles path = do
  withBinaryFile path ReadMode (\handle -> do
    contents <- B.hGetContents handle
    let words = B.unpack contents
    decodeFiles' words)


main = do
  (command:args) <- getArgs
  if command == "create" then do
    let (archiveName:fileNames) = args
    encodeFilesSeq archiveName fileNames
  else if command == "create-par" then do
    let (archiveName:fileNames) = args
    encodeFilesPar archiveName fileNames
  else if command == "extract" then do
    let (archiveName:_) = args
    decodeFiles archiveName
  else
    putStrLn "Unknown command."


-- get depths of all symbols in tree, with current level
getTreeDepths' :: Node -> Int -> [(Symbol, Int)]
getTreeDepths' (Interior l r _ _) n = getTreeDepths' l (n+1) ++ getTreeDepths' r (n+1)
getTreeDepths' (Leaf sym _) n = [(sym, n)]

getTreeDepths :: Node -> Map.Map Symbol Int
getTreeDepths n = Map.fromList (getTreeDepths' n 0)

-- get encodings of all symbols in tree, with current level
getAllEncodings' :: Node -> [Bool] -> [(Symbol, [Bool])]
getAllEncodings' (Leaf sym _) current = [(sym, current)]
getAllEncodings' (Interior l r _ _) current = getAllEncodings' l (False : current) ++ getAllEncodings' r (True : current)

getAllEncodings :: Node -> [(Symbol, [Bool])]
getAllEncodings tree = map (second reverse) (getAllEncodings' tree [])


-- get length of encoded file knowing each char's count and bit length
getEncodedLength :: Map.Map Symbol Int -> [(Word8, Int)] -> Int
getEncodedLength treeDepths symWeights = foldl 
  (\acc symWeight -> acc + (snd symWeight * fromMaybe 0 (Map.lookup (fst symWeight) treeDepths)))
  0 
  symWeights




toNum = fromIntegral . fromEnum

toInt :: (Integral a) => a -> Int
toInt = fromIntegral

-- pack 8 or less bools into a Word8, most significant bools first
packBools' :: [Bool] -> Word8 -> Int -> Word8
packBools' [b1,b2,b3,b4,b5,b6,b7,b8] _ _ = 
  toNum b1 `shiftL` 7 +
  toNum b2 `shiftL` 6 +
  toNum b3 `shiftL` 5 +
  toNum b4 `shiftL` 4 +
  toNum b5 `shiftL` 3 +
  toNum b6 `shiftL` 2 +
  toNum b7 `shiftL` 1 +
  toNum b8
packBools' [] word _ = word
packBools' (b:bs) word bitnum = packBools' bs ((toNum b `shiftL` (7 - bitnum)) + word) (bitnum + 1)

-- pack exactly 4 bytes into an Int, least significant bytes first
packBytes :: Word8 -> Word8 -> Word8 -> Word8 -> Int
packBytes w1 w2 w3 w4 = toInt w4 `shiftL` 24 + 
                        toInt w3 `shiftL` 16 + 
                        toInt w2 `shiftL` 8 + 
                        toInt w1

hPutWord32le :: (Integral n) => Handle -> n -> IO ()
hPutWord32le handle n = B.hPut handle (runPut (putWord32le (fromIntegral n)))

boolsToWords :: [Bool] -> [Word8]
boolsToWords (b1:b2:b3:b4:b5:b6:b7:b8:bs) = packBools' [b1,b2,b3,b4,b5,b6,b7,b8] 0 0 : boolsToWords bs
boolsToWords [] = []
boolsToWords bs = [packBools' bs 0 0]

-- read n bits from list of bytes, with starting bit number (should be 0 by default)
-- return n bits, and leftover words
wordsToBools' :: [Word8] -> Int -> Int -> ([Bool], [Word8])
wordsToBools' list@(b:bs) left bitnum
  | left == 0   = ([], bs)
  | bitnum == 8 = wordsToBools' bs left 0
  | otherwise   = first ((((b `shiftR` (7 - bitnum)) `mod` 2) == 1) :) (wordsToBools' list (left - 1) (bitnum + 1))
wordsToBools list left = wordsToBools' list left 0

decodeSingleFile :: [Word8] -> Node -> (B.ByteString, [Word8])
decodeSingleFile (w1:w2:w3:w4:file) tree = do
  let length = packBytes w1 w2 w3 w4
  let (encoded, rest) = wordsToBools file length
  (decode tree encoded, rest)


encodeNodes' :: Handle -> [Node] -> IO ()
encodeNodes' handle [] = return ()
encodeNodes' handle ((Leaf sym weight) : nodes) = do
  B.hPutStr handle (B.singleton sym)
  hPutWord32le handle weight
  encodeNodes' handle nodes

encodeNodes :: Handle -> [Node] -> IO ()
encodeNodes handle nodes = do
  hPutWord32le handle (length nodes)
  encodeNodes' handle nodes

decodeNodes' :: [Word8] -> [Node] -> Int -> ([Node], [Word8])
decodeNodes' rest nodes 0 = (nodes, rest)
decodeNodes' (sym:w1:w2:w3:w4:rest) nodes n = 
  decodeNodes' rest (Leaf sym (packBytes w1 w2 w3 w4) : nodes) (n - 1)
  
decodeNodes :: [Word8] -> ([Node], [Word8])
decodeNodes (w1:w2:w3:w4:ws) = first reverse (decodeNodes' ws [] (packBytes w1 w2 w3 w4))

encodeName :: Handle -> String -> IO ()
encodeName handle name = do
  hPutStr handle name
  hPutChar handle '\0'

decodeName' :: [Word8] -> String -> (String, [Word8])
decodeName' (w:words) currentString
  | w == 0    = (currentString, words)
  | otherwise = decodeName' words (chr (fromIntegral w) : currentString)

decodeName words = first reverse (decodeName' words [])
