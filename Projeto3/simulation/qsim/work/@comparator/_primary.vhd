library verilog;
use verilog.vl_types.all;
entity Comparator is
    port(
        A               : in     vl_logic_vector(8 downto 0);
        Output          : out    vl_logic
    );
end Comparator;
