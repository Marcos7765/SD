LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE IEEE.NUMERIC_STD.ALL;

ENTITY SAD_demo IS
PORT(
	reset, clk, clk50, go : STD_LOGIC;
	data_bus : INOUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	lcd_e, lcd_on, lcd_rw, lcd_rs, lcd_blon : OUT STD_LOGIC;
	info_state : OUT INTEGER RANGE 0 TO 5
);
END ENTITY SAD_demo;

ARCHITECTURE main OF SAD_demo IS

COMPONENT SAD IS
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
END COMPONENT;

COMPONENT BCD IS
PORT(
	num : IN INTEGER RANGE 0 TO 999;
	cent : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
	dez : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
	unid : OUT STD_LOGIC_VECTOR(3 DOWNTO 0)
);
END COMPONENT;

COMPONENT LCD_DISPLAY_nty IS
   PORT( 
      reset              : IN     std_logic;  -- Map this Port to a Switch within your [Port Declarations / Pin Planner]  
      clock_50           : IN     std_logic;  -- The DE2 50Mhz Clk and the "clk_count_400hz" counter variable are used to Genreate a 400Hz clock pulse 
                                              -- to drive the LCD CORE state machine.
      lcd_rs             : OUT    std_logic;
      lcd_e              : OUT    std_logic;
      lcd_rw             : OUT    std_logic;
      lcd_on             : OUT    std_logic;
      lcd_blon           : OUT    std_logic;
      
      data_bus_0         : INOUT  STD_LOGIC;
      data_bus_1         : INOUT  STD_LOGIC;
      data_bus_2         : INOUT  STD_LOGIC;
      data_bus_3         : INOUT  STD_LOGIC;
      data_bus_4         : INOUT  STD_LOGIC;
      data_bus_5         : INOUT  STD_LOGIC;
      data_bus_6         : INOUT  STD_LOGIC;
      data_bus_7         : INOUT  STD_LOGIC;
      
      LCD_CHAR_ARRAY_0   : IN    STD_LOGIC
   );
END COMPONENT;
SIGNAL sad_o : STD_LOGIC_VECTOR(31 DOWNTO 0);
BEGIN

sade : SAD PORT MAP(clk => clk, go=>go, reset => reset, sad=>sad_o, debug=>info_state);

LCDe : LCD_DISPLAY_nty PORT MAP(reset=>reset, clock_50=>clk50, lcd_rs=>lcd_rs, lcd_e=>lcd_e, lcd_rw=>lcd_rw, lcd_on=>lcd_on, lcd_blon=>lcd_blon, 
data_bus_0=>data_bus(0), data_bus_1=>data_bus(1),data_bus_2=>data_bus(2),data_bus_3=>data_bus(3),data_bus_4=>data_bus(4),data_bus_5=>data_bus(5),
data_bus_6=>data_bus(6),data_bus_7=>data_bus(7), LCD_CHAR_ARRAY_0 => '0');

END ARCHITECTURE main;