library verilog;
use verilog.vl_types.all;
entity Controller_vlg_check_tst is
    port(
        AB_rd           : in     vl_logic;
        debug           : in     vl_logic_vector(2 downto 0);
        i_clr           : in     vl_logic;
        i_inc           : in     vl_logic;
        sadreg_clr      : in     vl_logic;
        sadreg_ld       : in     vl_logic;
        sum_clr         : in     vl_logic;
        sum_ld          : in     vl_logic;
        sampler_rx      : in     vl_logic
    );
end Controller_vlg_check_tst;
