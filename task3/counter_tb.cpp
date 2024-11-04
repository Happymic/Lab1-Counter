#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // Init top Verilog instance
    Vcounter* top = new Vcounter; // This is the DUT!

    // Init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 3: Siuu");

    // Initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // Run simulation for many clock cycles
    for (i = 0; i < 1000; i++) {
        for (clk = 0; clk < 2; clk++) {
            // Dump variables into VCD file and toggle clock
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        
        // ++++ Send count value to Vbuddy using vbdHex
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);
        
        // ---end of Vbuddy output section
        /*
       // ++++ Send count value to Vbuddy using vbdPlot
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);
        // ---end of Vbuddy output section
        */
         

        // Change rst and en signals during simulation
        top->rst = (i < 2) | (i == 15);
        //top->en = (i > 4);
        top->en = vbdFlag(); //using flag as en signal
        
        vbdSetMode(1);

        if (Verilated::gotFinish()) exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}