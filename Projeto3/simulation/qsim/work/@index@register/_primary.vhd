library verilog;
use verilog.vl_types.all;
entity IndexRegister is
    port(
        clk             : in     vl_logic;
        clear           : in     vl_logic;
        increment       : in     vl_logic;
        data_out        : out    vl_logic_vector(7 downto 0)
    );
end IndexRegister;
