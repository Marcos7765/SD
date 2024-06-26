library verilog;
use verilog.vl_types.all;
entity BCD is
    port(
        num             : in     vl_logic_vector(7 downto 0);
        cent            : out    vl_logic_vector(3 downto 0);
        dez             : out    vl_logic_vector(3 downto 0);
        unid            : out    vl_logic_vector(3 downto 0)
    );
end BCD;
