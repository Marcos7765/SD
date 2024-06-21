library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity CustomAdder is
    Port (
        A : in STD_LOGIC_VECTOR(31 downto 0);
        B : in STD_LOGIC_VECTOR(7 downto 0);
        Result : out STD_LOGIC_VECTOR(31 downto 0)
    );
end CustomAdder;

architecture Behavioral of CustomAdder is
    signal B_extended : STD_LOGIC_VECTOR(31 downto 0);
begin
    process(A, B)
    begin
        B_extended <= (23 downto 0 => '0') & B;
        
        Result <= A + B_extended;
    end process;
end Behavioral;