Step 1: Loadable Counter
Pressing the switch on EC11 forces the counter to pre-set to Vbuddy's parameter value.

top->rst = vbdFlag();
top->value = vbdValue();
vbdSetMode(1); // one-shot behaviour


Step 2: Single Stepping
Increment the counter each time you press the switch.

top->rst = (i < 2) | (i == 15);
top->en = vbdFlag();
vbdSetMode(1); // one-shot behaviour