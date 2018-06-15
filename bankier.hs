
-- 0
max1 =      [[2, 1], [2, 3], [1, 1], [2, 1]]
przydz =    [[0, 0], [2, 1], [0, 1], [1, 1]]
dostepne =  [1, 1]

potrz = zipWith (\[x1, x2] [y1, y2] -> [x1 - y1, x2 - y2]) max1 przydz

-- 1


find_j koniec robocze j =
    if j < length koniec
    then if (j < length koniec) && ((koniec !! j) == False) && (potrz !! j <= robocze)
         then Just j
         else find_j koniec robocze (j + 1)
    else Nothing

set_true (x:xs) pos curr =
    if pos == curr
    then True : xs
    else x : (set_true xs) pos (curr + 1)

algo :: [Bool] -> [Integer] -> [Bool]
algo koniec robocze =
    case find_j koniec robocze 0 of
        Just j -> algo (set_true koniec j 0) (zipWith (+) (przydz !! j) robocze)
        Nothing -> koniec
algorun =
    let 
        koniec = [False | x <- [1 .. length przydz]]
        robocze = dostepne
        in 
    algo koniec robocze

