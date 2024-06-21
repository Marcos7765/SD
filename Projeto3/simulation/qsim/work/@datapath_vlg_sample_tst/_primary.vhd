library verilog;
use verilog.vl_types.all;
entity Datapath_vlg_sample_tst is
    port(
        A_data          : in     vl_logic_vector(7 downto 0);
        B_data          : in     vl_logic_vector(7 downto 0);
        clk             : in     vl_logic;
        index_clr       : in     vl_logic;
        index_inc       : in     vl_logic;
        sadreg_clr      : in     vl_logic;
        sadreg_ld       : in     vl_logic;
        sum_clr         : in     vl_logic;
        sum_ld          : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end Datapath_vlg_sample_tst;
