

remDups [] _ = []
remDups (x:xs) list2
    | (x `elem` list2) = remDups xs list2
    | otherwise = x : remDups xs (x:list2)

get_last requests =
    last (remDups requests [])

get_mem addr rs [] mem_max =
    ([addr], True)
get_mem addr rs mem@(m:ms) mem_max
    | addr `elem` mem =
        (mem, False)
    | length mem < mem_max = 
        (mem ++ [addr], True)
    | otherwise =
        let last = get_last rs
            in
        ((filter ((/=) last) mem) ++ [addr], True)

count_mems [] mem mem_max start = start
count_mems requests@(r:rs) mem mem_max start =
    case get_mem r rs mem mem_max of 
        (new_mem, True) -> count_mems rs new_mem mem_max (start + 1)
        (new_mem, False) -> count_mems rs new_mem mem_max start

count_memz requests mem_max =
    count_mems requests [] mem_max 0
