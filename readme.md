# EDPS (Industrial PLC Data Acquisition System)

## Overview

**EDPS** is a modular, legacy industrial data polling and supervision system designed to bridge the gap between plant-floor programmable logic controllers (PLCs) and enterprise database environments. Originally developed to operate within robust industrial environments, this application collects raw data, monitors trends, and captures critical process events (such as batch start/stop sequences, QA samples, and flag-based alerts) from industrial networks.

The system is built with a decoupled architecture, ensuring fault-tolerant data logging, hardware-independent communication, and asynchronous database writing without interrupting critical PLC polling cycles.

## Key Features

* **Hardware-Independent Polling:** Interfaces with heterogeneous industrial sources using a unified, hardware-independent interface.
* **Multi-Protocol Communication:** Communicates with PLCs via standard industrial protocols, evolving from **InTouch DDE** at the project's inception to **SAPI-S7** and **OPC UA** as communication technologies advanced.
* **Asynchronous Data Queueing:** Uses dedicated queues for read/write jobs and historical data buffering, ensuring the primary polling engine is never blocked by database latency.
* **Fault-Tolerant Architecture:** Implements a Local Engine Interface System (LEIS) connected to a local SQL Server to buffer historical data, guaranteeing zero data loss in the event of an enterprise network outage.
* **Flexible HMI Integration & Testing:** Integrates seamlessly with HMI applications (like Wonderware InTouch) for real-time control and supervision. Additionally, its VBA-compatible communication libraries allow MS Excel to be used as a lightweight HMI complement for testing and research without requiring expensive HMI application development.

## System Architecture

The application is highly modular, splitting distinct responsibilities into separate executables and DLLs that communicate via OLE and ADO.

### Core Modules:

* **EDPSRun (Application Starter):** The bootstrapper that sequentially initializes and runs all system modules based on configuration parameters defined in `EDPS.INI`.
* **SIS (Supervision Interface System):** The "brain" of the application. It acts as the active coordinator. It reads instruction codes and PLC addresses from a local database. It then determines necessary events, manages data flows, and creates asynchronous queues for historical data buffering. It features a cyclic reader driven by a Loop Parameters Table for scanning event flags from PLCs.
* **FDIS/SAPI/OpcS7 (Foreign Data Interface System/Simple Application Programming Interface/Open Platform Communications):** This system is a communication layer of the Supervision Interface System. It provides a middle tier of data exchange between the PLCs and EDPS, evolving from DDE at the beginning of the project to SAPI-S7 and OPC UA as new communication technologies became available. FDIS handles communication via an InTouch DDE server. SAPI acts as a client communicating over the S7 Simple Application Programming Interface, and OpcS7 is based on the hardware-independent Open Platform Communications Unified Architecture. An additional benefit of this library is that it is compatible with VBA; combined with MS Excel, it provides HMI-complementary functionality for testing and research without requiring expensive HMI application development.
* **DBIS (DataBase Interface System):** Manages the synchronous ADO connection to the primary MS SQL Server for dataset manipulation and control info updates.
* **QR (Query Reader):** A bridge module that prepares SQL queries to be performed asynchronously, passing them down the pipeline.
* **LEIS (Local Engine Interface System):** The local buffering engine. It receives prepared queries and stores them in a local SQL Server database, guaranteeing zero data loss during enterprise network outages.

## Repository Structure

```text
📦 EDPS
 ┣ 📂 Doc          # System architecture diagrams and functional schemes
 ┣ 📂 edpsrun      # Source/Binaries for the Application Starter module
 ┣ 📂 FDIS         # Source/Binaries for the Foreign Data Interface System
 ┣ 📂 LEIS         # Source/Binaries for the Local Engine Interface System
 ┣ 📂 OpcS7        # Source/Binaries for the Open Platform Communications for S7 Client 
 ┣ 📂 SIS          # Source/Binaries for the Siemens Interface System
 ┗ 📂 SAPI         # Source/Binaries for the Siemens S7 Simple Application Programming Interface Client

```
## Data Flow & Workflow

1. **Data Ingestion:** `FDIS` polls the PLC using the configured protocol, via one of a SAPI module, based on the timer loops defined in `SIS`.
2. **Supervision & Routing:** `SIS` evaluates the raw data. If it detects a critical event (batch stop, QA sample), it coordinates the data flow.
3. **Local Buffering:** Historical data is passed via OLE to the `LEIS`, which safely queues it into a Local SQL Server DB via ADO.
4. **Enterprise Sync:** `QR` and `DBIS` work in tandem to asynchronously push the buffered data from the local engine up to the primary MS SQL Server over the Office LAN.
5. **HMI Feedback:** The HMI (InTouch Application) can read from the DBIS/HWIS layers to display real-time statuses and send control requests back through the `SIS`.

## Historical Context

This repository serves as a portfolio piece demonstrating classic enterprise-grade industrial IT architecture. It showcases foundational skills in system decoupling, localized data buffering for fault tolerance, and low-level memory/queue management across separate executables.