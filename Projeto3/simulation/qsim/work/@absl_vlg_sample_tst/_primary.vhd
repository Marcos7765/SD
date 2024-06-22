library verilog;
use verilog.vl_types.all;
entity Absl_vlg_sample_tst is
    port(
        A               : in     vl_logic_vector(8 downto 0);
        sampler_tx      : out    vl_logic
    );
end Absl_vlg_sample_tst;
