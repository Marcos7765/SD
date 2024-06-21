library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity DataRegister is
    Port (
        clk : in STD_LOGIC;
        load : in STD_LOGIC;
        clear : in STD_LOGIC;
        data_in : in STD_LOGIC_VECTOR(31 downto 0);
        data_out : out STD_LOGIC_VECTOR(31 downto 0)
    );
end DataRegister;

architecture Behavioral of DataRegister is
begin
    process(clk)
    begin
        if rising_edge(clk) then
            if clear = '1' then
                data_out <= (others => '0');
            elsif load = '1' then
                data_out <= data_in;
            end if;
        end if;
    end process;
end Behavioral;

