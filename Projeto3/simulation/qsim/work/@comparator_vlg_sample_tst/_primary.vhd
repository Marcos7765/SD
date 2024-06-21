library verilog;
use verilog.vl_types.all;
entity Comparator_vlg_sample_tst is
    port(
        A               : in     vl_logic_vector(8 downto 0);
        sampler_tx      : out    vl_logic
    );
end Comparator_vlg_sample_tst;
