library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity Absl is
    Port (
        A : in STD_LOGIC_VECTOR(7 downto 0);
        Result : out STD_LOGIC_VECTOR(7 downto 0)
    );
end Absl;

architecture Behavioral of Absl is
begin
    process(A)
    begin
        if A(7) = '1' then
            Result <= -A;
        else
            Result <= A;
        end if;
    end process;
end Behavioral;

