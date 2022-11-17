#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" 

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vcounter* top = new Vcounter;

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    if (vbdOpen() != 1) return (-1);
    vbdHeader("L1T3: One-Shot");
    //

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    for (i = 0; i < 300; i++) {
        
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk); // unit is in ps!!!
            top->clk = !top->clk;
            top->eval();
        }
        
        // Output count values to 7-seg display every cycle
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        
        vbdCycle(i + 1);
        // ---- end of Vbuddy output section

        // Edit 
        top->en = vbdFlag();
        vbdSetMode(1); // one-shot behaviour
        
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose(); // housekeeping
    tfp->close();
    exit(0);

}
