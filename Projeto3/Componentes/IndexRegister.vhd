library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity IndexRegister is
    Port (
        clk : in STD_LOGIC;
        clear : in STD_LOGIC;
        increment : in STD_LOGIC;
        data_out : out STD_LOGIC_VECTOR(8 downto 0)
    );
end IndexRegister;

architecture Behavioral of IndexRegister is
    signal Creg : integer RANGE 0 to 2**10 -1;
begin
    process(clk, clear)
    begin
			if (clear='1') then
				Creg <= 0;
			elsif (rising_edge(clk) AND increment ='1') then
                Creg <= Creg + 1;
         end if;
    end process;
    data_out <= std_LOGIC_VECTOR(to_unsigned(Creg, 9));
end architecture Behavioral;