import Data.Bits
import Data.Binary.Put
import qualified Data.ByteString.Lazy as B
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

decode :: Node -> Node -> [Bool] -> [Symbol]
decode codeTree (Leaf s w) bits =
  s : decode codeTree codeTree bits
decode codeTree (Interior l _ _ _) (False : bits) =
  decode codeTree l bits
decode codeTree (Interior _ r _ _) (True : bits) =
  decode codeTree r bits
decode _ _ [] = []

encode :: Node -> Node -> [Symbol] -> [Bool]
encode _ _ [] = []
encode codeTree (Leaf s w) (sym : symbols) =
  encode codeTree codeTree symbols
encode codeTree (Interior l r _ _) (sym : symbols)
  | sym `elem` getSymbols l = False : encode codeTree l (sym : symbols)
  | otherwise = True : encode codeTree r (sym : symbols)


scanFile :: FilePath -> IO [(Char, Int)]
scanFile path = withFile path ReadMode (\handle -> do
  contents <- hGetContents handle
  -- lazy IO, must force reading the whole file with $!!
  return $!! Map.toList (Map.fromListWith (+) (map (\x -> (x, 1)) contents)))

toNodes :: [(Char, Int)] -> [Node]
toNodes symWeights = sortBy (compare `on` weight) (map (\(sym, weight) -> Leaf sym weight) symWeights)

encodeFile :: FilePath -> IO ()
encodeFile path = do
  symWeights <- scanFile path
  let nodes = toNodes symWeights
  let tree = makeTree nodes

  encoded <- withFile path ReadMode (\handle -> do
    contents <- hGetContents handle
    return $!! encode tree tree contents)

  let treeDepths = getTreeDepths tree
  let fileLength = getEncodedLength treeDepths symWeights
  withFile (path ++ ".ha") WriteMode (\handle -> do
    B.hPut handle (runPut (putWord32le (fromIntegral fileLength)))
    let byteString = B.pack (boolsToWords encoded)
    B.hPut handle byteString)

main = do
  (command:args) <- getArgs
  if command == "encode" then do
    let (filename:_) = args
    encodeFile filename
  else
    putStrLn "Unknown command."

-- get depths of all symbols in tree, with current level
getTreeDepths' :: Node -> Int -> [(Symbol, Int)]
getTreeDepths' (Interior l r _ _) n = getTreeDepths' l (n+1) ++ getTreeDepths' r (n+1)
getTreeDepths' (Leaf sym _) n = [(sym, n)]

getTreeDepths :: Node -> Map.Map Symbol Int
getTreeDepths n = Map.fromList (getTreeDepths' n 0)


-- get length of encoded file knowing each char's count and bit length
getEncodedLength :: Map.Map Symbol Int -> [(Char, Int)] -> Int
getEncodedLength treeDepths symWeights = foldl 
  (\acc symWeight -> acc + (snd symWeight * (maybe 0 id (Map.lookup (fst symWeight) treeDepths))))
  0 
  symWeights


toNum = fromIntegral . fromEnum

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

boolsToWords :: [Bool] -> [Word8]
boolsToWords (b1:b2:b3:b4:b5:b6:b7:b8:bs) = packBools' [b1,b2,b3,b4,b5,b6,b7,b8] 0 0 : boolsToWords bs
boolsToWords [] = []
boolsToWords bs = [packBools' bs 0 0]
