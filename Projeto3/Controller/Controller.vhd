LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY Controller IS
PORT(
		clk, reset, go: IN STD_LOGIC;
		i_lt_256: IN STD_LOGIC;
		AB_rd: OUT STD_LOGIC;
		i_inc, i_clr: OUT STD_LOGIC;
		sum_ld, sum_clr: OUT STD_LOGIC;
		sadreg_ld, sadreg_clr: OUT STD_LOGIC;
		debug: OUT INTEGER RANGE 0 TO 5
);
END ENTITY Controller;

ARCHITECTURE main OF Controller IS
	TYPE states IS (S0, S1, S2, S3, S4, RST);
	SIGNAL stateReg : states := S0;
	SIGNAL nextState : states;

BEGIN
	PROCESS (clk, reset)
	BEGIN
		IF (reset = '1') THEN
			stateReg <= RST; 
		ELSIF rising_edge(clk) THEN
			stateReg <= nextState;
		END IF;
	END PROCESS;
	
	PROCESS (stateReg, go, i_lt_256)
	BEGIN
		CASE stateReg IS
			WHEN S0 =>
				AB_rd <= '0';
				sum_clr <= '0';
				sum_ld <= '0';
				i_inc <= '0';
				i_clr <= '0';
				sadreg_ld <= '0';
				sadreg_clr <= '0';
				debug <= 0;
				IF (go ='1') THEN
					nextState <= S1;
				ELSE
					nextState <= S0;
				END IF;
			WHEN S1 =>
				AB_rd <= '0';
				sum_clr <= '1';
				sum_ld <= '0';
				i_inc <= '0';
				i_clr <= '1';
				sadreg_ld <= '0';
				sadreg_clr <= '0';
				debug <= 1;
				nextState <= S2;
			WHEN S2 =>
				AB_rd <= '0';
				sum_clr <= '0';
				sum_ld <= '0';
				i_inc <= '0';
				i_clr <= '0';
				sadreg_ld <= '0';
				sadreg_clr <= '0';
				debug <= 2;
				IF (i_lt_256 = '1') THEN
					nextState <= S3;
				ELSE
					nextState <= S4;
				END IF;
			WHEN S3 =>
				AB_rd <= '1';
				sum_clr <= '0';
				sum_ld <= '1';
				i_inc <= '1';
				i_clr <= '0';
				sadreg_ld <= '0';
				sadreg_clr <= '0';
				debug <= 3;
				nextState <= S2;
			WHEN S4 =>
				AB_rd <= '0';
				sum_clr <= '0';
				sum_ld <= '0';
				i_inc <= '0';
				i_clr <= '0';
				sadreg_ld <= '1';
				sadreg_clr <= '0';
				debug <= 4;
				nextState <= S0;
			--extra state for reset
			WHEN RST =>
				AB_rd <= '0';
				sum_clr <= '1';
				sum_ld <= '0';
				i_inc <= '0';
				i_clr <= '1';
				sadreg_ld <= '0';
				sadreg_clr <= '1';
				debug <= 5;
				nextState <= S0;
			END CASE;
	END PROCESS;

END ARCHITECTURE main;