library verilog;
use verilog.vl_types.all;
entity SAD_vlg_check_tst is
    port(
        AB_addr_watch   : in     vl_logic_vector(8 downto 0);
        debug           : in     vl_logic_vector(2 downto 0);
        sad             : in     vl_logic_vector(31 downto 0);
        sampler_rx      : in     vl_logic
    );
end SAD_vlg_check_tst;
