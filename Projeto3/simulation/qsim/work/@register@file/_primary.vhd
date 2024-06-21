library verilog;
use verilog.vl_types.all;
entity RegisterFile is
    port(
        clk             : in     vl_logic;
        wr_en           : in     vl_logic;
        rd_addr         : in     vl_logic_vector(7 downto 0);
        wr_addr         : in     vl_logic_vector(7 downto 0);
        data_in         : in     vl_logic_vector(7 downto 0);
        data_out        : out    vl_logic_vector(7 downto 0)
    );
end RegisterFile;
