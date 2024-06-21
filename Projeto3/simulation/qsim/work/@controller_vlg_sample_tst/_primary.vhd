library verilog;
use verilog.vl_types.all;
entity Controller_vlg_sample_tst is
    port(
        clk             : in     vl_logic;
        go              : in     vl_logic;
        i_lt_256        : in     vl_logic;
        reset           : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end Controller_vlg_sample_tst;
