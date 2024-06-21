library verilog;
use verilog.vl_types.all;
entity Absl_vlg_check_tst is
    port(
        Result          : in     vl_logic_vector(7 downto 0);
        sampler_rx      : in     vl_logic
    );
end Absl_vlg_check_tst;
