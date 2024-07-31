//
// Created by qiuyuhang on 24-7-30.
//
#include "common_data_bus.hpp"

void Cdb::erase(int entry, bool memAddr) {
    for (int i = 0; i < capacity; i++) {
        if (cdb[i].entry == entry && cdb[i].memAddr == memAddr) {
//            assert(cdb[i].state == USED);
            cdbNext[i].init();
            return;
        }
    }
    assert(0); // entry not found
}
void Cdb::eraseBr(int entry) {
    for (int i = 0; i < capacity; i++) {
        if (br[i].entry == entry) {
//            assert(br[i].state == USED);
            brNext[i].init();
            return;
        }
    }
}
void Cdb::flush() {
#ifdef detail
    std::cout << "cdb flush: nonebr " << std::endl;
    for (int i = 0; i < capacity; i++) {
        if (cdb[i].state == LEISURE && cdbNext[i].state == LEISURE) {
            continue;
        }
        std::cout << "cdb: " << i << std::endl;
        cdb[i].print();
        cdbNext[i].print();
    }
    std::cout << "br flush: " << std::endl;
    for (int i = 0; i < capacity; i++) {
        if (br[i].state == LEISURE && brNext[i].state == LEISURE) {
            continue;
        }
        std::cout << "br: " << i << std::endl;
        br[i].print();
        brNext[i].print();
    }
#endif
    for (int i = 0; i < capacity; i++) {
        cdb[i] = cdbNext[i];
    }
    ju = juNext;
    for (int i = 0; i < capacity; i++) {
        br[i] = brNext[i];
    }
}

void Cdb::writeBr(int pc, bool rsl, int entry) {
    for (int i = 0; i < capacity; i++) {
        if (br[i].entry == entry) {
            assert(0);
        }
    }
    for (int i = 0; i < capacity; i++) {
        if (br[i].state == LEISURE) {
            brNext[i].pc = pc;
            brNext[i].rsl = rsl;
            brNext[i].entry = entry;
            brNext[i].state = ACTIVE;
            return;
        }
    }
    assert(0); // br is full
}

void Cdb::write(int value, int entry, bool memAddr) {

    for (int i = 0; i < capacity; i++) {
        if (cdb[i].state==LEISURE&&cdb[i].entry == entry && cdb[i].memAddr == memAddr) {
#ifdef detail
            for (int j = 0; j < capacity; ++j) {
                cdb[j].print();
            }
#endif
            assert(0);
        }
    }
    for (int i = 0; i < capacity; i++) {
        if (cdb[i].state == LEISURE) {
            cdbNext[i].value = value;
            cdbNext[i].entry = entry;
            cdbNext[i].state = ACTIVE;
            cdbNext[i].memAddr = memAddr;
            return;
        }
    }
    assert(0); // cdb is full
}

std::pair<int, int> Cdb::get(int entry, bool memAddr) {
    for (int i = 0; i < capacity; i++) {
        if (cdb[i].entry == entry && cdb[i].memAddr == memAddr) {
            cdbNext[i].state = USED;
            auto rsl = std::make_pair(true, cdb[i].value);
            if (memAddr) {
                erase(entry, true);
            }
            return rsl;
        }
    }
    return std::make_pair(false, 0);
}

std::pair<int, std::pair<int, int>> Cdb::getBr(int entry) {//<找到，《pc，rsl》>
    for (int i = 0; i < capacity; i++) {
        if (br[i].entry == entry) {
            brNext[i].state = USED;
            return std::make_pair(true, std::make_pair(br[i].pc, br[i].rsl));
        }
    }
    return std::make_pair(false, std::make_pair(0, false));
}

void Cdb::writeJal(int value, int entry, int valuePC) {
    if (ju.entry == entry) {
        assert(0);
    }
    if (ju.state == LEISURE) {
        juNext.value = value;
        juNext.entry = entry;
        juNext.valuePC = valuePC;
        juNext.state = ACTIVE;
        return;
    }
    assert(0); // ju is full
}

std::pair<int, int> Cdb::getJal(int entry) {
    if (ju.entry == entry) {
        juNext.state = USED;
        return std::make_pair(true, ju.value);
    }
    return std::make_pair(false, 0);
}