library verilog;
use verilog.vl_types.all;
entity BCD_vlg_sample_tst is
    port(
        num             : in     vl_logic_vector(7 downto 0);
        sampler_tx      : out    vl_logic
    );
end BCD_vlg_sample_tst;
