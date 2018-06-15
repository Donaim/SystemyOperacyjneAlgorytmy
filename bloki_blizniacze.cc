#include <iostream>
#include <vector>

using namespace std;

struct block {
    block(int tmem) : total_mem(tmem), used_mem(0), pid(-1), free(true) {}
    block(int tmem, int umem, int id) : total_mem(tmem), used_mem(umem), pid(id), free(false) {}
    int
        total_mem,
        used_mem,
        pid;
    bool free;
};

const int max_mem = 1 << 10;
vector< block > mem;

void show_mem() {
    for (auto & p : mem) {
        cout << (p.free ? " " : to_string(p.pid)) << ' ' << p.total_mem << endl;
    }
    cout << endl;
}

int find_mem(int size, int start_index = 0) {
    for (int i = start_index; i < mem.size(); i++) {
        if (mem[i].free && mem[i].total_mem == size) {
            return i;
        }
    }
    return -1;
}
int find_free(int size) {
    for (int i = 0; i < mem.size(); i++) {
        if (mem[i].free && mem[i].total_mem > size) {
            return i;
        }
    }
    return -1;
}
int split_to(int size) {
    while(find_mem(size) == -1) {
        int first_free = find_free(size);
        if (first_free == -1) { throw runtime_error("process is too big to fit into available memory"); }

        auto half = block{mem[first_free].total_mem / 2};
        mem.erase(mem.begin() + first_free);
        mem.insert(mem.begin() + first_free, half);
        mem.insert(mem.begin() + first_free, half);
    }
}
void create_process(int need, int id) {
    int pow2 = 2;
    while (pow2 < need) {
        pow2 *= 2;
    }

    int find = find_mem(pow2);
    if (find == -1) {
        split_to(pow2);
        find = find_mem(pow2);
    }

    mem[find] = block{pow2, need, id};
}
bool merge_one_free() {
    for (int i = 0; i < mem.size(); i++) {
        if (!mem[i].free) { continue; }
        int find = find_mem(mem[i].total_mem, i + 1);
        if (find != -1) {
            int new_size = mem[i].total_mem * 2;

            mem.erase(mem.begin() + i);
            mem.erase(mem.begin() + find);

            mem.push_back(block{new_size});
            return true;
        }
    }
    return false;
}
int find_pid(int pid) {
    for (int i = 0; i < mem.size(); i++) {
        if (mem[i].pid == pid) { return i; }
    }
    return -1;
}
void delete_process(int pid) {
    int mem_index = find_pid(pid);
    if (mem_index == -1) { throw invalid_argument{"wrong pid: " + to_string(pid)}; }
    mem[mem_index] = block{mem[mem_index].total_mem};
    merge_one_free();
    // while(merge_one_free());
}

int main()
{
    mem.push_back(block{max_mem});
    cout << "max_mem=" << max_mem << endl;

    int processes[] = {100, 240, 64, 250};

    for (int i = 0; i < sizeof(processes) / sizeof(int); i++) {
        create_process(processes[i], i);
        show_mem();
    }
    for (int i = 0; i < 3; i++) {
        delete_process(i);
        show_mem();
    }

    return 0;
}
