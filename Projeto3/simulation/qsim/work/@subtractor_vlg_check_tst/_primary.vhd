library verilog;
use verilog.vl_types.all;
entity Subtractor_vlg_check_tst is
    port(
        Result          : in     vl_logic_vector(8 downto 0);
        sampler_rx      : in     vl_logic
    );
end Subtractor_vlg_check_tst;
