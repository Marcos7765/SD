library verilog;
use verilog.vl_types.all;
entity BCD_vlg_check_tst is
    port(
        cent            : in     vl_logic_vector(3 downto 0);
        dez             : in     vl_logic_vector(3 downto 0);
        unid            : in     vl_logic_vector(3 downto 0);
        sampler_rx      : in     vl_logic
    );
end BCD_vlg_check_tst;
