library verilog;
use verilog.vl_types.all;
entity DataRegister_vlg_sample_tst is
    port(
        clear           : in     vl_logic;
        clk             : in     vl_logic;
        data_in         : in     vl_logic_vector(31 downto 0);
        load            : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end DataRegister_vlg_sample_tst;
