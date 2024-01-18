-- lab3.vhd

-- Generated using ACDS version 18.1 625

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity lab3_top is
	port (
		clk                             : in    std_logic                    := '0'; --                       clk.clk
		scl : inout std_logic                    := '0'; --  opencores_i2c_0_export_0.scl_pad_io
		sda : inout std_logic                    := '0'; --                          .sda_pad_io
		seg0 : out std_logic_vector(6 downto 0);
		seg1 : out std_logic_vector(6 downto 0);
		seg2 : out std_logic_vector(6 downto 0);
		seg3 : out std_logic_vector(6 downto 0);
		bouton : in    std_logic;  
		reset                   : in  std_logic                    := '0'  --                     reset.reset_n
	);
end entity lab3_top;

architecture rtl of lab3_top is
	component lab3 is
		port (
			clk_clk                               : in    std_logic                    := '0'; --                            clk.clk
			opencores_i2c_0_export_0_scl_pad_io   : inout std_logic                    := '0'; --       opencores_i2c_0_export_0.scl_pad_io
			opencores_i2c_0_export_0_sda_pad_io   : inout std_logic                    := '0'; --                               .sda_pad_io
			pio_0_external_connection_export      : out   std_logic_vector(3 downto 0);        --      pio_0_external_connection.export
			pio_1_external_connection_export      : out   std_logic_vector(3 downto 0);        --      pio_1_external_connection.export
			pio_2_external_connection_export      : out   std_logic_vector(3 downto 0);        --      pio_2_external_connection.export
			pio_3_external_connection_export      : out   std_logic_vector(3 downto 0);        --      pio_3_external_connection.export
			pio_bouton_external_connection_export : in    std_logic                    := '0'; -- pio_bouton_external_connection.export
			reset_reset_n                         : in    std_logic                    := '0'  --                          reset.reset_n
		);
		end component lab3;

		component bin_to_7seg is
			port (
			   bin_in : in  STD_LOGIC_VECTOR (3 downto 0);
			   seg_out : out  STD_LOGIC_VECTOR (6 downto 0));
	   end component bin_to_7seg;

	signal s0 : std_logic_vector(3 downto 0);
	signal s1 : std_logic_vector(3 downto 0);
	signal s2 : std_logic_vector(3 downto 0);
	signal s3 : std_logic_vector(3 downto 0);
begin
		u0 : component lab3
			port map (
				clk_clk                               => clk,                               --                            clk.clk
				opencores_i2c_0_export_0_scl_pad_io   => scl,   --       opencores_i2c_0_export_0.scl_pad_io
				opencores_i2c_0_export_0_sda_pad_io   => sda,   --                               .sda_pad_io
				pio_0_external_connection_export      => s0,      --      pio_0_external_connection.export
				pio_1_external_connection_export      => s1,      --      pio_1_external_connection.export
				pio_2_external_connection_export      => s2,      --      pio_2_external_connection.export
				pio_3_external_connection_export      => s3,      --      pio_3_external_connection.export
				reset_reset_n                         => reset,                         --                          reset.reset_n
				pio_bouton_external_connection_export => bouton  -- pio_bouton_external_connection.export
			);
		
		u1 : component bin_to_7seg
			port map (
				bin_in                     => s0,  
				seg_out                    => seg0
			);
		u2 : component bin_to_7seg
			port map (
				bin_in                     => s1,  
				seg_out                    => seg1
			);
		u3 : component bin_to_7seg
			port map (
				bin_in                     => s2,  
				seg_out                    => seg2
			);
		u4 : component bin_to_7seg
			port map (
				bin_in                     => s3,  
				seg_out                    => seg3
			);
end architecture rtl;
