#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" // insert Vbuddy code here (Changes 1)

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

    // init Vbuddy (Changes 2) - Open and initialize Vbuddy connection. Port path is in vbuddy.cfg
    if (vbdOpen() != 1) return (-1);
    vbdHeader("Lab 1: Counter");
    //

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i = 0; i < 300; i++) {
        
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk); // unit is in ps!!!
            top->clk = !top->clk;
            top->eval();
        }
        
        // ++++ Send count value to Vbuddy (Changes 3) 
        // Output count values to 7-seg display every cycle
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        // ----end of Vbuddy output section

        // Plot on the TFT - plotting a dot is much faster than outputting to the 7-segment display
        // start/stop the counter with the flag
        // vbdPlot(int(top->count), 0, 255);
        
        vbdCycle(i + 1);
        // ---- end of Vbuddy output section

        top->rst = (i < 2) | (i == 15);
        top->en = vbdFlag();
        if (Verilated::gotFinish()) exit(0);
    }

    // House keeping (Changes 4)
    vbdClose(); // ++++

    tfp->close();
    exit(0);

}