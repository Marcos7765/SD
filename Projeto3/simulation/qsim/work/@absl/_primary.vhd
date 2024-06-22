library verilog;
use verilog.vl_types.all;
entity Absl is
    port(
        A               : in     vl_logic_vector(8 downto 0);
        Result          : out    vl_logic_vector(7 downto 0)
    );
end Absl;
