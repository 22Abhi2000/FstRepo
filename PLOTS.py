# -*- coding: utf-8 -*-
"""
Created on Tue Sep 16 13:54:12 2025

@author: Adminservice
"""

# -*- coding: utf-8 -*-
"""
Created on Tue Sep 16 13:27:01 2025

@author: Adminservice
"""

# -*- coding: utf-8 -*-
"""
Created on Thu Sep  4 16:06:49 2025

@author: Adminservice
"""

import os
import re
import csv
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
import numpy as np  # For calculating the square root

# Reference values for x, y, z
x_ref = 1324532.9440 #km
y_ref = 6074656.0820 #km
z_ref = 1421894.0360 #km

# Reference values for vx, vy, vz
vx_ref = 0.0 #km
vy_ref = 0.0 #km
vz_ref = 0.0 #km

# Define xlim for the plots
xlim = (pd.to_datetime("18:30:00.055"), pd.to_datetime("18:52:58.364"))




# ----------------- USER CONFIG -----------------
FILES_AND_HEADERS = [
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Time_of_Week_-second-_for_SV_TM1MC1_P.epo", "SPS Second"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Week_Number_TM1MC1_P.epo", "SPS WeekNumber"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Time_of_Week_-nano_seconds-_for_SV_TM1MC1_P.epo","SPS NanoSecond"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Tme_Sync_-SPS_system_of_week_-week_number-_TM1MC1_P.epo","Sync WeekNumber"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Tme_Sync_-SPS_system_of_week_-second-_TM1MC1_P.epo","Sync Second"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Tme_Sync_-_time_of_week_-nano_seconds-_TM1MC1_P.epo","Sync NanoSecond"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_PPS_Week_Number_TM1MC1_P.epo","PPS WeekNumber"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_PPS_Time_of_Week_-second-_for_SV_TM1MC1_P.epo","PPS Second"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_PPS_Time_of_Week_-nano_seconds-_for_SV_TM1MC1_P.epo","PPS NanoSecond"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Position_x-co-ordinate_TM1MC1_P.epo","SPS X(POS)"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Position_y-co-ordinate_TM1MC1_P.epo","SPS Y(POS)"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Position_z-co-ordinate_TM1MC1_P.epo","SPS Z(POS)"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Velocity_x-direction_TM1MC1_P.epo","SPS VEL_X"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Velocity_y-direction_TM1MC1_P.epo","SPS VEL_Y"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SPS_Velocity_z-direction_TM1MC1_P.epo","SPS VEL_Z"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\number_of_satellites_tracked_TM1MC1_P.epo","No of Sat tracked"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\PDOP_TM1MC1_P.epo","PDOP"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Clock_Bias_TM1MC1_P.epo","Clock Bais"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Clock_Drift_TM1MC1_P.epo","Clock Drift"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Inter_satellite_bias_TM1MC1_P.epo","Inter Satellite Bias"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\inter_satellite_drift_TM1MC1_P.epo","Inter Satellite Drift"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\HWDT_reset_flag_TM1MC1_P.epo","HWDT Reset Flag"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SWDT_RESET_flag_TM1MC1_P.epo","SWDT Reset Flag"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Hardware_reset_counter_TM1MC1_P.epo","Hardware_reset_Counter"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Software_reset_counter_TM1MC1_P.epo","Software_reset_Counter"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_1_TM1MC1_P.epo","SVID 1"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_2_TM1MC1_P.epo","SVID 2"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_3_TM1MC1_P.epo","SVID 3"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_4_TM1MC1_P.epo","SVID 4"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_5_TM1MC1_P.epo","SVID 5"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_6_TM1MC1_P.epo","SVID 6"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_7_TM1MC1_P.epo","SVID 7"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_8_TM1MC1_P.epo","SVID 8"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_9_TM1MC1_P.epo","SVID 9"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_10_TM1MC1_P.epo","SVID 10"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_11_TM1MC1_P.epo","SVID 11"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_12_TM1MC1_P.epo","SVID 12"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_13_TM1MC1_P.epo","SVID 13"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_14_TM1MC1_P.epo","SVID 14"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_15_TM1MC1_P.epo","SVID 15"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\SVID_of_Channel_number_16_TM1MC1_P.epo","SVID 16"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_1_TM1MC1_P.epo","CNDR 1"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_2_TM1MC1_P.epo","CNDR 2"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_3_TM1MC1_P.epo","CNDR 3"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_4_TM1MC1_P.epo","CNDR 4"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_5_TM1MC1_P.epo","CNDR 5"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_6_TM1MC1_P.epo","CNDR 6"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_7_TM1MC1_P.epo","CNDR 7"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_8_TM1MC1_P.epo","CNDR 8"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_9_TM1MC1_P.epo","CNDR 9"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_10_TM1MC1_P.epo","CNDR 10"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_11_TM1MC1_P.epo","CNDR 11"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_12_TM1MC1_P.epo","CNDR 12"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_13_TM1MC1_P.epo","CNDR 13"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_14_TM1MC1_P.epo","CNDR 14"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_15_TM1MC1_P.epo","CNDR 15"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_16_TM1MC1_P.epo","CNDR 16"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_17_TM1MC1_P.epo","CNDR 17"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\CNDR_of_Channel_number_18_TM1MC1_P.epo","CNDR 18"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-1_TM1MC1_P.epo","Elve 1"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-2_TM1MC1_P.epo","Elve 2"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-3_TM1MC1_P.epo","Elve 3"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-4_TM1MC1_P.epo","Elve 4"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-5_TM1MC1_P.epo","Elve 5"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-6_TM1MC1_P.epo","Elve 6"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-7_TM1MC1_P.epo","Elve 7"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-8_TM1MC1_P.epo","Elve 8"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-9_TM1MC1_P.epo","Elve 9"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-10_TM1MC1_P.epo","Elve 10"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-11_TM1MC1_P.epo","Elve 11"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-12_TM1MC1_P.epo","Elve 12"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-13_TM1MC1_P.epo","Elve 13"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-14_TM1MC1_P.epo","Elve 14"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-15_TM1MC1_P.epo","Elve 15"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-16_TM1MC1_P.epo","Elve 16"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-17_TM1MC1_P.epo","Elve 17"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Elevation_for_Channel-18_TM1MC1_P.epo","Elve 18"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Lock_Indicator_GPS-1_TM1MC1_P.epo","Lock Status 1"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Lock_Indicator_GPS-2_TM1MC1_P.epo","Lock Status 2"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Lock_Indicator_GPS-3_TM1MC1_P.epo","Lock Status 3"),
    (r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\TM1MC1PData\Lock_Indicator_GPS-4_TM1MC1_P.epo","Lock Status 4"),
    
    
]

index_ref = 1  # 2nd file is primary
FINAL_ORDER = None
OUTPUT_CSV = r"D:\TEJASWINI APPRENTICE\DATA FILES\LLA_PRE_TEST\Combined.csv"
# ------------------------------------------------

TIMELINE_COL = "SYSTEM TIME"

# Regex: match lines like "17:02:14.460   37.0000"
LINE_RE = re.compile(r"^\s*(\d{2}:\d{2}:\d{2}\.\d+)\s+(.+?)\s*$")

def read_epo_to_df(path: str, value_col_name: str) -> pd.DataFrame:
    """Parse one .epo/.eop file → DataFrame with:
      - SYSTEM TIME (string hh:mm:ss.sss…)
      - value_col_name (string, preserve trailing zeros)
    """
    if not os.path.exists(path):
        raise FileNotFoundError(f"File not found: {path}")

    rows = []
    with open(path, "r", encoding="utf-8", errors="ignore") as fh:
        for ln in fh:
            m = LINE_RE.match(ln)
            if not m:
                continue
            ts = m.group(1)       # keep full timestamp as string
            val = m.group(2).strip()
            rows.append((ts, val))

    # Force SYSTEM TIME + value column as string
    df = pd.DataFrame(rows, columns=[TIMELINE_COL, value_col_name], dtype=str)

    if not df.empty:
        df = df.drop_duplicates(subset=[TIMELINE_COL], keep="first").reset_index(drop=True)
    return df

def parse_time_for_sort(ts: str):
    """Convert timestamp string to datetime object for sorting (supports ms/us)."""
    return datetime.strptime(ts, "%H:%M:%S.%f")

def merge_epo_files(files_and_headers, index_ref=1, final_order=None, output_csv="merged.csv") -> pd.DataFrame:
    """Merge multiple .epo/.eop files on SYSTEM TIME."""
    if not files_and_headers:
        raise ValueError("No files provided in files_and_headers.")

    dfs = []
    for path, col in files_and_headers:
        if not os.path.exists(path):
            base, ext = os.path.splitext(path)
            alt = base + ".epo" if ext.lower() in [".eop", ".txt", ""] else None
            if alt and os.path.exists(alt):
                path_to_use = alt
            else:
                raise FileNotFoundError(f"File not found: {path}")
        else:
            path_to_use = path
        df = read_epo_to_df(path_to_use, col)
        dfs.append(df)

    # Build master timeline (union of all timestamps)
    all_ts = set()
    for df in dfs:
        if not df.empty:
            all_ts.update(df[TIMELINE_COL].tolist())
    if not all_ts:
        raise ValueError("No timestamped data found in any file.")

    # Sort chronologically using datetime
    sorted_ts = sorted(all_ts, key=parse_time_for_sort)
    master = pd.DataFrame({TIMELINE_COL: sorted_ts}, dtype=str)

    # Merge each DF onto master
    merged = master
    for df in dfs:
        merged = pd.merge(merged, df, on=TIMELINE_COL, how="left")

    merged = merged.fillna("NaN")

    # Column order
    if final_order:
        desired_order = final_order.copy()
        if desired_order[0] != TIMELINE_COL:
            desired_order.insert(0, TIMELINE_COL)
    else:
        ref_idx = index_ref if 0 <= index_ref < len(files_and_headers) else 0
        ref_col = files_and_headers[ref_idx][1]
        others = [col for i, (_, col) in enumerate(files_and_headers) if i != ref_idx]
        desired_order = [TIMELINE_COL, ref_col] + others

    for col in desired_order:
        if col not in merged.columns:
            merged[col] = ""
    merged = merged[desired_order]
    merged[TIMELINE_COL] = merged[TIMELINE_COL].astype(str)

    # Save CSV with quotes
    merged.to_csv(output_csv, index=False, quoting=csv.QUOTE_ALL)

    print(f"✅ Saved merged CSV to: {output_csv}")
    print("Preview (first 10 rows):")
    print(merged.head(30).to_string(index=False))

    return merged

#------------------PPS Time(WN)-------------------------------

def pps_wn_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "PPS WeekNumber" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["PPS WeekNumber"], errors='coerce'), marker='o', linestyle="",color='b', label="PPS Week Number")
    
    plt.title("LLA_PRE_TEST-PPS WeekNumber of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("PPS WeekNumber")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "pps_wn.png")
    plt.savefig(plot_filename)

    plt.show()

#------------------PPS Time(Second)-------------------------------

def pps_sec_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "PPS Second" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["PPS Second"], errors='coerce'), marker='o',linestyle="", color='b', label="PPS Second")
    
    plt.title("LLA_PRE_TEST-PPS Seconds of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("PPS Second(sec)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "pps_sec.png")
    plt.savefig(plot_filename)

    plt.show()
    
#------------------SPS Time(WN)-------------------------------

def sps_wn_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "SPS WeekNumber" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["SPS WeekNumber"], errors='coerce'), marker='o', color='b', label="SPS Week Number")
    
    plt.title("LLA_PRE_TEST-SPS WeekNumber of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("SPS WeekNumber")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "sps_wn.png")
    plt.savefig(plot_filename)

    plt.show()

#------------------SPS Time(Second)-------------------------------

def sps_sec_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "SPS Second" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["SPS Second"], errors='coerce'), marker='o',linestyle="", color='b', label="SPS Second(sec)")
    
    plt.title("LLA_PRE_TEST-SPS Seconds of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("SPS Second(sec)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "sps_sec.png")
    plt.savefig(plot_filename)

    plt.show()
    
#------------------SYN Time(WN)-------------------------------

def syn_wn_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "Sync WeekNumber" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["Sync WeekNumber"], errors='coerce'), marker='o', color='b', label="Sync WeekNumber")
    
    plt.title("LLA_PRE_TEST-SYN WeekNumber of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("Sync WeekNumber")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "syn_wn.png")
    plt.savefig(plot_filename)

    plt.show()

#------------------SPS Time(Second)-------------------------------

def syn_sec_plot(df: pd.DataFrame,xlim=None,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "Sync Second" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["Sync Second"], errors='coerce'), marker='o',  linestyle="",color='b', label="Sync Second(sec)")
    
    plt.title("LLA_PRE_TEST-SYN Seconds of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("Sync Second(sec)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    
    if xlim:
        plt.xlim(xlim)  # Apply xlim if provided

    plt.tight_layout()

    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "syn_sec.png")
    plt.savefig(plot_filename)
    
    

    plt.show()

#------------------Clock Bais-------------------------------

def clock_bais_plot(df: pd.DataFrame, min_val=-30, max_val=30,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "Clock Bais" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["Clock Bais"], errors='coerce'), marker='x', linestyle="",color='purple', label="Clock Bias")
    
    plt.title("LLA_PRE_TEST Clock Bias of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("Clock Bias(mts)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Set y-axis limits to specified range
    plt.ylim(min_val, max_val)
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "clock_bais.png")
    plt.savefig(plot_filename)

    plt.show()
    
#------------------Clock Drift-------------------------------

def clock_drift_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    """
    Plot clockbais values against SYSTEM TIME.
    """
    plt.figure(figsize=(12, 6))
    if "Clock Drift" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["Clock Drift"], errors='coerce'), marker='x',linestyle="", color='purple', label="Clock drift")
    
    plt.title("LLA_PRE_TEST Clock Drift of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("Clock Drift(m/s)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "clock_dift.png")
    plt.savefig(plot_filename)

    plt.show()
    
#---------------------- PDOP AND NO OF SAT --------------------
    
def calculate_and_pdop_and_no_of_sat(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    
    """
    Plot SVID and number of satellites values against SYSTEM TIME.

    Parameters:
    df : pandas.DataFrame
        The input dataframe containing the data.
    timeline_col : str
        The column name representing the SYSTEM TIME.
    """
    # Ensure that the SYSTEM TIME column is in datetime format
    df[TIMELINE_COL] = pd.to_datetime(df[TIMELINE_COL], errors='coerce')

    # Set up the plot
    plt.figure(figsize=(12, 6))

    # Plot SVID if the column exists
    if "PDOP" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["PDOP"], errors='coerce')/100, marker='o',linestyle="", color='red', label="PDOP")

    # Plot number of satellites if the column exists
    if "No of Sat tracked" in df.columns:
        plt.plot(df[TIMELINE_COL], pd.to_numeric(df["No of Sat tracked"], errors='coerce'), marker='x',linestyle="", color='blue', label="Number of Satellites")
    
    # Adding title, labels, and other plot customization
    plt.title("LLA_PRE_TEST PDOP and No of tracked Sat of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("Values")
    plt.xticks(rotation=10)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path, "pdop_no-of-sat.png")
    plt.savefig(plot_filename)

    # Show plot
    plt.show()
    
# ----------------- STEP 4: PLOT CNDR GROUPS -----------------
def plot_cndr_groups(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    df[TIMELINE_COL] = pd.to_datetime(df[TIMELINE_COL])

    groups = [(1,6),(7,12),(13,18)]
    for start, end in groups:
        plt.figure(figsize=(12, 6))
        for i in range(start, end + 1):
            col = f"CNDR {i}"
            if col not in df.columns:
                continue
            plt.plot(df[TIMELINE_COL], pd.to_numeric(df[col], errors="coerce"), linestyle="", marker='x',label=col)

        plt.title(f"LLA_PRE_TEST CNDR{start}-CNDR{end} of Acoustics Test for GYN TVD2")
        plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
        plt.ylabel("CNDR Value(dBHz)")
        plt.xticks(rotation=45)
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        # Save the plot
        os.makedirs(save_path, exist_ok=True)
        plot_filename = os.path.join(save_path, f"cndr{start}-cndr{end}.png")
        plt.savefig(plot_filename)

       
       
        plt.show()
# ----------------- STEP 4: PLOT ELEVATION GROUPS -----------------
def plot_elev_groups(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
    df[TIMELINE_COL] = pd.to_datetime(df[TIMELINE_COL], format="%H:%M:%S.%f")

    groups = [(1,6),(7,12),(13,18)]
    for start, end in groups:
        plt.figure(figsize=(12, 6))
        for i in range(start, end + 1):
            col = f"Elve {i}"
            if col not in df.columns:
                continue
            plt.plot(df[TIMELINE_COL], pd.to_numeric(df[col], errors="coerce"), marker='x',linestyle='', label=col)

        plt.title(f"LLA_PRE_TEST Elev{start}-Elev{end} Accuracy of Acoustics Test for GYN TVD2")
        plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
        plt.ylabel("Elevation Value(deg)")
        plt.xticks(rotation=45)
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        
        # Save the plot
        os.makedirs(save_path, exist_ok=True)
        plot_filename = os.path.join(save_path, f"elev{start}-elev{end}.png")
        plt.savefig(plot_filename)
       
       
        plt.show()

# Function to calculate differences and delta_cal, then plot the graph
def rss_pos_plot_differences(df: pd.DataFrame, x_ref: float, y_ref: float, z_ref: float,save_path="LLA_PRE_TEST_PLOT"):
    """
    Calculate the differences between the reference values and the corresponding columns in the DataFrame,
    calculate delta_cal (Euclidean distance), then plot these differences (x_diff, y_diff, z_diff, delta_cal) 
    on a single graph against SYSTEM TIME.
    """
    # Calculate differences for x, y, z
    df['x_diff'] = x_ref - pd.to_numeric(df['SPS X(POS)'], errors='coerce')
    df['y_diff'] = y_ref - pd.to_numeric(df['SPS Y(POS)'], errors='coerce')
    df['z_diff'] = z_ref - pd.to_numeric(df['SPS Z(POS)'], errors='coerce')

    # Calculate delta_cal (Euclidean distance)
    df['RSS_POS'] = np.sqrt(df['x_diff']**2 + df['y_diff']**2 + df['z_diff']**2)

    # Convert SYSTEM TIME to datetime for proper plotting
    df[TIMELINE_COL] = pd.to_datetime(df[TIMELINE_COL], format="%H:%M:%S.%f")

    # Plot differences and delta_cal on a single graph
    plt.figure(figsize=(12, 6))

    # Plot x_diff
    plt.plot(df[TIMELINE_COL], df['x_diff'], marker='o', linestyle='-', color='r', label='delta x')

    # Plot y_diff
    plt.plot(df[TIMELINE_COL], df['y_diff'], marker='o', linestyle='-', color='g', label='delta y')

    # Plot z_diff
    plt.plot(df[TIMELINE_COL], df['z_diff'], marker='o', linestyle='-', color='b', label='delta z')

    # Plot delta_cal
    plt.plot(df[TIMELINE_COL], df['RSS_POS'], marker='x', linestyle='-', color='purple', label='RSS_POS')

    # Add labels and title
    plt.title("LLA_PRE_TEST Pos Accuracy of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("POS_Accuracy(mts)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
     # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path,"POS_Accuracy.png")
    plt.savefig(plot_filename)

    # Show the plot
    plt.show()
   

    # Save the updated DataFrame (with differences and delta_cal) to the CSV file
    df.to_csv(OUTPUT_CSV, index=False, quoting=csv.QUOTE_ALL)
    print(f"✅ Updated CSV with differences and RSS POS saved to: {OUTPUT_CSV}")

def rss_pos_plot(df: pd.DataFrame,save_path="LLA_PRE_TEST_PLOT"):
# Plot delta_cal
    plt.plot(df[TIMELINE_COL], df['RSS_POS'], marker='x', linestyle='-', color='red', label='RSS_POS')
   
    # Add labels and title
    plt.title("LLA_PRE_TEST RSS Pos Accuracy of Acoustics Test for GYN G1")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("RSS_POS(mts)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    
     # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path,"RSS_POS.png")
    plt.savefig(plot_filename)

    # Show the plot
    plt.show()

    
# Function to calculate differences and delta_cal, then plot the graph
def rss_vel_plot_differences(df: pd.DataFrame, vx_ref: float, vy_ref: float, vz_ref: float,min_val=-3,max_val=3,save_path="LLA_PRE_TEST_PLOT"):
    """
    Calculate the differences between the reference values and the corresponding columns in the DataFrame,
    calculate delta_cal (Euclidean distance), then plot these differences (x_diff, y_diff, z_diff, delta_cal) 
    on a single graph against SYSTEM TIME.
    """
    # Calculate differences for x, y, z
    df['vx_diff'] = vx_ref - pd.to_numeric(df['SPS VEL_X'], errors='coerce')
    df['vy_diff'] = vy_ref - pd.to_numeric(df['SPS VEL_Y'], errors='coerce')
    df['vz_diff'] = vz_ref - pd.to_numeric(df['SPS VEL_Z'], errors='coerce')

    # Calculate delta_cal (Euclidean distance)
    df['RSS_VEL'] = np.sqrt(df['vx_diff']**2 + df['vy_diff']**2 + df['vz_diff']**2)

    # Convert SYSTEM TIME to datetime for proper plotting
    df[TIMELINE_COL] = pd.to_datetime(df[TIMELINE_COL], format="%H:%M:%S.%f")

    # Plot differences and delta_cal on a single graph
    plt.figure(figsize=(12, 6))

    # Plot x_diff
    plt.plot(df[TIMELINE_COL], pd.to_numeric(df["vx_diff"], errors='coerce')*100, marker='o',linestyle="", color='r', label='delta vx')

    # Plot y_diff
    plt.plot(df[TIMELINE_COL], pd.to_numeric(df["vy_diff"], errors='coerce')*100, marker='o',linestyle="", color='g', label='delta vy')

    # Plot z_diff
    plt.plot(df[TIMELINE_COL], pd.to_numeric(df["vz_diff"], errors='coerce')*100, marker='o', linestyle="", color='b', label='delta vz')

    # Plot delta_cal-
    plt.plot(df[TIMELINE_COL], pd.to_numeric(df["RSS_VEL"], errors='coerce')*100, marker='x',linestyle="", color='purple', label='RSS_VEL')

    # Add labels and title
    plt.title("LLA_PRE_TEST Vel Accuracy of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("VEL_Accuracy(cm/s)")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    # Set y-axis limits to specified range
    plt.ylim(min_val, max_val)
    # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path,"VEL_Accuracy.png")
    plt.savefig(plot_filename)
    # Show the plot
    plt.show()

    # Save the updated DataFrame (with differences and delta_cal) to the CSV file
    df.to_csv(OUTPUT_CSV, index=False, quoting=csv.QUOTE_ALL)
    print(f"✅ Updated CSV with differences and RSS Vel saved to: {OUTPUT_CSV}")
    
def rss_vel_plot(df: pd.DataFrame,min_val=0,max_val=3,save_path="LLA_PRE_TEST_PLOT"):
     # Plot delta_cal
    plt.plot(df[TIMELINE_COL],  pd.to_numeric(df["RSS_VEL"], errors='coerce')*100, marker='x', linestyle='', color='red', label='RSS_VEL')
    
    # Add labels and title
    plt.title("LLA_PRE_TEST RSS Vel Accuracy of Acoustics Test for GYN TVD2")
    plt.xlabel(f"{TIMELINE_COL} (HH:MM:SS)")
    plt.ylabel("RSS_VEL(cm/sec")
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.ylim(min_val, max_val)
     # Save the plot
    os.makedirs(save_path, exist_ok=True)
    plot_filename = os.path.join(save_path,"RSS_VEL.png")
    plt.savefig(plot_filename)
    
    plt.show()



if __name__ == "__main__":
    # Merge files and generate the combined DataFrame
    df_merged = merge_epo_files(
        FILES_AND_HEADERS, index_ref=index_ref, final_order=FINAL_ORDER, output_csv=OUTPUT_CSV
)




# PLOTS

rss_pos_plot_differences(df_merged, x_ref, y_ref, z_ref)
rss_pos_plot(df_merged)
rss_vel_plot_differences(df_merged, vx_ref, vy_ref, vz_ref)
rss_vel_plot(df_merged)
plot_cndr_groups(df_merged)
plot_elev_groups(df_merged)
clock_bais_plot(df_merged)
clock_drift_plot(df_merged)
pps_wn_plot(df_merged)
pps_sec_plot(df_merged)
sps_wn_plot(df_merged)
sps_sec_plot(df_merged)
syn_wn_plot(df_merged)
syn_sec_plot(df_merged)
calculate_and_pdop_and_no_of_sat(df_merged)

