library verilog;
use verilog.vl_types.all;
entity Datapath_vlg_check_tst is
    port(
        AB_addr         : in     vl_logic_vector(8 downto 0);
        index_lt_256    : in     vl_logic;
        sad             : in     vl_logic_vector(31 downto 0);
        sampler_rx      : in     vl_logic
    );
end Datapath_vlg_check_tst;
