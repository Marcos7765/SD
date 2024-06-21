library verilog;
use verilog.vl_types.all;
entity SAD is
    port(
        clk             : in     vl_logic;
        go              : in     vl_logic;
        reset           : in     vl_logic;
        sad             : out    vl_logic_vector(31 downto 0);
        debug           : out    vl_logic_vector(2 downto 0);
        AB_addr_watch   : out    vl_logic_vector(8 downto 0)
    );
end SAD;
