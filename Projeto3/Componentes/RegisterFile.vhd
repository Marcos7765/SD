library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity RegisterFile is
    GENERIC (
		mode : NATURAL := 0
	 );
	 Port (
        clk : in STD_LOGIC;
        wr_en : in STD_LOGIC;
        rd_addr : in STD_LOGIC_VECTOR(7 downto 0);
        wr_addr : in STD_LOGIC_VECTOR(7 downto 0);
        data_in : in STD_LOGIC_VECTOR(7 downto 0);
        data_out : out STD_LOGIC_VECTOR(7 downto 0)
    );
end RegisterFile;

architecture Behavioral of RegisterFile is
    type mem_array is array (0 to 255) of STD_LOGIC_VECTOR(7 downto 0);
    signal RF : mem_array := (others => (others => '0'));
	 constant A : mem_array := (
		  0 => "11111111",
		  1 => "11111111",
		  2 => "11111111",
		  3 => "11111111",
		  4 => "11111111",
		  5 => "11111111",
        others => (others => '0')
    );
	 constant B : mem_array := (
		  0 => "00000000",
		  1 => "11111111",
		  2 => "00000000",
		  3 => "11111111",
		  4 => "00000000",
		  5 => "11111111",
        others => (others => '0')
    );
begin
    process(clk)
    begin
      if (rising_edge(clk)) then
			if (wr_en='1') then
				RF(to_integer(unsigned(wr_addr))) <= data_in;
			end if;
		end if;
    end process;
	 
	 modo: IF (mode = 0) GENERATE
		data_out <= RF(to_integer(unsigned(rd_addr)));
	 END GENERATE modo;
	 mA: IF (mode = 1) GENERATE
		data_out <= A(to_integer(unsigned(rd_addr)));
	 END GENERATE mA;
	 mB: IF (mode = 2) GENERATE
		data_out <= B(to_integer(unsigned(rd_addr)));
	 END GENERATE mB;
	 
end architecture Behavioral;