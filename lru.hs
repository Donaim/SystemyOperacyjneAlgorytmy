

get_mem addr [] mem_max =
    ([addr], True)
get_mem addr mem@(m:ms) mem_max
    | addr `elem` mem =
        ((filter ((/=) addr) mem) ++ [addr], False)
    | length mem < mem_max = 
        (mem ++ [addr], True)
    | otherwise =
        (ms ++ [addr], True)

count_mems [] mem mem_max start = start
count_mems requests@(r:rs) mem mem_max start =
    case get_mem r mem mem_max of 
        (new_mem, True) -> count_mems rs new_mem mem_max (start + 1)
        (new_mem, False) -> count_mems rs new_mem mem_max start

count_memz requests mem_max =
    count_mems requests [] mem_max 0