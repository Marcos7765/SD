LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY SAD IS
GENERIC(
	A_mode : NATURAL := 1;
	B_mode : NATURAL := 2;
	cmp_target : NATURAL := 8
);
PORT(
	clk, go, reset : IN STD_LOGIC;
	sad: OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
	debug: OUT INTEGER RANGE 0 TO 5;
	AB_addr_watch: OUT STD_LOGIC_VECTOR(8 DOWNTO 0)
);
END ENTITY SAD;

ARCHITECTURE main OF SAD IS

component Controller is
PORT(
		clk, reset, go: IN STD_LOGIC;
		i_lt_256: IN STD_LOGIC;
		AB_rd: OUT STD_LOGIC;
		i_inc, i_clr: OUT STD_LOGIC;
		sum_ld, sum_clr: OUT STD_LOGIC;
		sadreg_ld, sadreg_clr: OUT STD_LOGIC;
		debug: OUT INTEGER RANGE 0 TO 5
);
end component;

component Datapath is
	 GENERIC (cmp_target : NATURAL := cmp_target);
    Port (
        clk : in STD_LOGIC;
        index_inc : in STD_LOGIC;
        index_clr : in STD_LOGIC;
        sum_ld : in STD_LOGIC;
        sum_clr : in STD_LOGIC;
        sadreg_ld : in STD_LOGIC;
        sadreg_clr : in STD_LOGIC;
        A_data : in STD_LOGIC_VECTOR(7 downto 0);
        B_data : in STD_LOGIC_VECTOR(7 downto 0);
        sad : out STD_LOGIC_VECTOR(31 downto 0);
        AB_addr : out STD_LOGIC_VECTOR(8 downto 0);
        index_lt_256 : out STD_LOGIC
    );
end component;

component RegisterFile is
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
end component;

	SIGNAL AB_rd_o, i_inc_o, i_clr_o, sum_ld_o, sum_clr_o, sadreg_ld_o, sadreg_clr_o : STD_LOGIC;

	SIGNAL AB_addr_o : STD_LOGIC_VECTOR(8 downto 0);
	SIGNAL i_lt_256_o : STD_LOGIC;

	SIGNAL A_data_out_o, B_data_out_o : STD_LOGIC_VECTOR(7 downto 0);

BEGIN

	CTRL : Controller PORT MAP (clk => clk, reset => reset, go => go, i_lt_256 => i_lt_256_o, AB_rd => AB_rd_o,
		i_inc => i_inc_o, i_clr => i_clr_o, sum_ld => sum_ld_o, sum_clr => sum_clr_o, sadreg_ld => sadreg_ld_o,
		sadreg_clr => sadreg_clr_o, debug => debug);	
	
	DP : Datapath PORT MAP (clk => clk, index_inc => i_inc_o, index_clr => i_clr_o, sum_ld => sum_ld_o,
		sum_clr => sum_clr_o, sadreg_ld => sadreg_ld_o, sadreg_clr => sadreg_clr_o, A_data => A_data_out_o,
		B_data => B_data_out_o, sad => sad, AB_addr => AB_addr_o, index_lt_256 => i_lt_256_o);
		
	A_RF : RegisterFile GENERIC MAP (mode => A_mode) PORT MAP (clk => clk, wr_en => '0', rd_addr => AB_addr_o (7 downto 0), wr_addr => (others => '0'),
		data_in => (others => '0'), data_out => A_data_out_o);
		
	B_RF : RegisterFile GENERIC MAP (mode => B_mode) PORT MAP (clk => clk, wr_en => '0', rd_addr => AB_addr_o (7 downto 0), wr_addr => (others => '0'),
		data_in => (others => '0'), data_out => B_data_out_o);
		
	AB_addr_watch <= AB_addr_o;

END ARCHITECTURE main;