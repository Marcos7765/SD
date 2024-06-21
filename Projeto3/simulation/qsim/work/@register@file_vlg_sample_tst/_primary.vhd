library verilog;
use verilog.vl_types.all;
entity RegisterFile_vlg_sample_tst is
    port(
        clk             : in     vl_logic;
        data_in         : in     vl_logic_vector(7 downto 0);
        rd_addr         : in     vl_logic_vector(7 downto 0);
        wr_addr         : in     vl_logic_vector(7 downto 0);
        wr_en           : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end RegisterFile_vlg_sample_tst;
