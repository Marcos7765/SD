LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

ENTITY BCD IS
PORT(
	num : IN INTEGER RANGE 999 DOWNTO 0;
	cent : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
	dez : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
	unid : OUT STD_LOGIC_VECTOR(3 DOWNTO 0)
);
END BCD;

ARCHITECTURE main OF BCD IS
SIGNAL tmp_cent : INTEGER RANGE 9 DOWNTO 0;
SIGNAL tmp_dez : INTEGER RANGE 9 DOWNTO 0;
SIGNAL tmp_unid : INTEGER RANGE 9 DOWNTO 0;
BEGIN
	PROCESS(num)
	BEGIN
		tmp_cent <= num/100;
		tmp_dez <= (num-(100*tmp_cent))/10;
		tmp_unid <= (num-(100*tmp_cent + 10*tmp_dez));
	END PROCESS;
	cent <= STD_LOGIC_VECTOR(to_unsigned(tmp_cent,4));
	dez <= STD_LOGIC_VECTOR(to_unsigned(tmp_dez,4));
	unid <= STD_LOGIC_VECTOR(to_unsigned(tmp_unid,4));
END ARCHITECTURE main;