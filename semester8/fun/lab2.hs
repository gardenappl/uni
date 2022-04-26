import Data.Bifunctor
import Data.Bits
import Data.Binary.Get
import Data.Binary.Put
import qualified Data.ByteString.Lazy as B
import Data.Char
import Data.List
import qualified Data.Map as Map
import Data.Maybe
import Data.Function (on)
import Data.Word
import System.Environment
import System.IO
import Control.DeepSeq (($!!))

type Symbol = Char

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
  let n =
        Interior
          first
          second
          (getSymbols first ++ getSymbols second)
          (getWeight first + getWeight second)
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

decode tree = decode' tree tree

-- Encode symbols into bool list based on tree
encode' :: Node -> Node -> [Symbol] -> [Bool]
encode' _ _ [] = []
encode' codeTree (Leaf s w) (sym : symbols) =
  encode' codeTree codeTree symbols
encode' codeTree (Interior l r _ _) (sym : symbols)
  | sym `elem` getSymbols l = False : encode' codeTree l (sym : symbols)
  | otherwise = True : encode' codeTree r (sym : symbols)

encode tree = encode' tree tree


-- First pass: get all symbols in file and their weights, as list
scanFile :: FilePath -> IO [(Char, Int)]
scanFile path = withFile path ReadMode (\handle -> do
  contents <- hGetContents handle
  -- lazy IO, must force reading the whole file with $!!
  return $!! Map.toList (Map.fromListWith (+) (map (\x -> (x, 1)) contents)))

toNodes :: [(Char, Int)] -> [Node]
toNodes symWeights = sortBy (compare `on` weight) (map (uncurry Leaf) symWeights)

hPutWord32le :: (Integral n) => Handle -> n -> IO ()
hPutWord32le handle n = B.hPut handle (runPut (putWord32le (fromIntegral n)))

encodeFilesSync' :: Handle -> [FilePath] -> IO ()
encodeFilesSync' _ [] = return ()
encodeFilesSync' handle (filePath:files) = do
  symWeights <- scanFile filePath
  putStrLn filePath
  let nodes = toNodes symWeights
  print nodes
  let tree = makeTree nodes
  print (getAllEncodings tree)
  let treeDepths = getTreeDepths tree

  encodeName handle filePath
  encodeNodes handle nodes

  let fileLength = getEncodedLength treeDepths symWeights
  hPutWord32le handle fileLength

  encoded <- withFile filePath ReadMode (\handle -> do
    contents <- hGetContents handle
    return $!! encode tree contents)
  B.hPut handle (B.pack (boolsToWords encoded))
  encodeFilesSync' handle files 

encodeFilesSync :: FilePath -> [FilePath] -> IO ()
encodeFilesSync archivePath files = withBinaryFile archivePath WriteMode (\handle ->
  encodeFilesSync' handle files)


decodeFiles' :: [Word8] -> IO ()
decodeFiles' [] = return ()
decodeFiles' words = do
  let (name, nodeBytes) = decodeName words
  putStr "Name: "
  putStrLn name
  let (nodes, fileBytes) = decodeNodes nodeBytes
  print nodes
  let tree = makeTree nodes
  print (getAllEncodings tree)
  let (file, rest) = decodeSingleFile fileBytes tree
  writeFile name file
  decodeFiles' rest


decodeFiles :: FilePath -> IO ()
decodeFiles path = do
  withBinaryFile path ReadMode (\handle -> do
    contents <- B.hGetContents handle
    let words = B.unpack contents
    decodeFiles' words)


divCeil :: (Integral a) => a -> a -> a
divCeil a b = a `div` b + (if a `mod` b > 0 then 1 else 0)

main = do
  (command:args) <- getArgs
  if command == "encode" then do
    let (archiveName:fileNames) = args
    encodeFilesSync archiveName fileNames
  else if command == "decode" then do
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
getAllEncodings tree = map (\(sym, path) -> (sym, reverse path)) (getAllEncodings' tree [])


-- get length of encoded file knowing each char's count and bit length
getEncodedLength :: Map.Map Symbol Int -> [(Char, Int)] -> Int
getEncodedLength treeDepths symWeights = foldl 
  (\acc symWeight -> acc + (snd symWeight * (fromMaybe 0 (Map.lookup (fst symWeight) treeDepths))))
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

decodeSingleFile :: [Word8] -> Node -> ([Symbol], [Word8])
decodeSingleFile (w1:w2:w3:w4:file) tree = do
  let length = packBytes w1 w2 w3 w4
  let (encoded, rest) = wordsToBools file length
  (decode tree encoded, rest)


encodeNodes' :: Handle -> [Node] -> IO ()
encodeNodes' handle [] = return ()
encodeNodes' handle ((Leaf sym weight) : nodes) = do
  hPutChar handle sym
  hPutWord32le handle weight
  encodeNodes' handle nodes

encodeNodes :: Handle -> [Node] -> IO ()
encodeNodes handle nodes = do
  hPutWord32le handle (length nodes)
  encodeNodes' handle nodes

decodeNodes' :: [Word8] -> [Node] -> Int -> ([Node], [Word8])
decodeNodes' rest nodes 0 = (nodes, rest)
decodeNodes' (sym:w1:w2:w3:w4:rest) nodes n = 
  decodeNodes' rest (Leaf (chr (fromIntegral sym)) (packBytes w1 w2 w3 w4) : nodes) (n - 1)
  
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
