library verilog;
use verilog.vl_types.all;
entity Datapath is
    port(
        clk             : in     vl_logic;
        index_inc       : in     vl_logic;
        index_clr       : in     vl_logic;
        sum_ld          : in     vl_logic;
        sum_clr         : in     vl_logic;
        sadreg_ld       : in     vl_logic;
        sadreg_clr      : in     vl_logic;
        A_data          : in     vl_logic_vector(7 downto 0);
        B_data          : in     vl_logic_vector(7 downto 0);
        sad             : out    vl_logic_vector(31 downto 0);
        AB_addr         : out    vl_logic_vector(8 downto 0);
        index_lt_256    : out    vl_logic
    );
end Datapath;
