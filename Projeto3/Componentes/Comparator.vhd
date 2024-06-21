library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
--use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Comparator is
    GENERIC(
		cmp_target : natural := 256
	 );
	 Port (
        A : in STD_LOGIC_VECTOR(8 downto 0);
        Output : out STD_LOGIC
    );
end Comparator;

architecture Behavioral of Comparator is
begin
    process(A)
    begin
        if (to_integer(unsigned(A)) < cmp_target) then
            Output <= '1';
        else
            Output <= '0';
        end if;
    end process;
end Behavioral;

