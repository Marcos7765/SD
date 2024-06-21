library verilog;
use verilog.vl_types.all;
entity Controller is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        go              : in     vl_logic;
        i_lt_256        : in     vl_logic;
        AB_rd           : out    vl_logic;
        i_inc           : out    vl_logic;
        i_clr           : out    vl_logic;
        sum_ld          : out    vl_logic;
        sum_clr         : out    vl_logic;
        sadreg_ld       : out    vl_logic;
        sadreg_clr      : out    vl_logic;
        debug           : out    vl_logic_vector(2 downto 0)
    );
end Controller;
