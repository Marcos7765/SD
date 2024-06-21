library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Subtractor is
    Port (
        A : in STD_LOGIC_VECTOR(7 downto 0);
        B : in STD_LOGIC_VECTOR(7 downto 0);
        Result : out STD_LOGIC_VECTOR(7 downto 0)
    );
end Subtractor;

architecture Behavioral of Subtractor is
begin
    process(A, B)
    begin
        Result <= A - B;
    end process;
end Behavioral;

