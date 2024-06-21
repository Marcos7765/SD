library verilog;
use verilog.vl_types.all;
entity IndexRegister_vlg_sample_tst is
    port(
        clear           : in     vl_logic;
        clk             : in     vl_logic;
        increment       : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end IndexRegister_vlg_sample_tst;
