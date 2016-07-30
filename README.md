# rtableau
This package uses Rcpp to connect R to the [Tableau SDK](https://onlinehelp.tableau.com/current/api/sdk/en-us/help.htm).

The Tableau SDK functionalities are implemented in this package as two R functions:
- `write_tde`: Take a R data.frame and output it as a Tableau Data Extract (.tde) file
- `publish_tde`: Publish a .tde file to a Tableau Server

## Set-up
I am still working on the configure file so you may run into issues during installation.  You may need to create your own Makevars if you installed in a different location than I did.
Install the [Tableau SDK](https://onlinehelp.tableau.com/current/api/sdk/en-us/help.htm#SDK/tableau_sdk_installing.htm) before trying to install this package.  
I've only tested this on Mac and Linux, here's where my SDK files are installed. 

#### Linux  
Shared object files: `/usr/lib64/tableausdk/`  
Binary executable: `/usr/local/bin/tdeserver64` 

#### Mac  
`/Library/Frameworks`

## Known Issues
- The Tableau SDK spawns other processes. **This can cause havoc with GUIs such as RStudio.** Therefore rtableau should only be used in console R.
- There are some issues with the 9.3 SDK on Linux when using large datasets, I recommend using the 9.2 version of the SDK which is more stable.
- The Tableau SDK does not support SSL on Linux for publishing


## Data Type Mapping
The main challenge of implementing this is that a R data.frame can have arbitrary data structures and types, while C++ is strongly typed.  I try to map types but keep in mind you will run into issues if you try and output anything other than a matrix of scalar values.

R vector type | Rcpp vector type | Tableau scalar type
------------ | ------------- | -------------
character | CharacterVector | CharString
factor | CharacterVector | CharString
double | NumericVector | Double
integer | IntegerVector | Integer
logical | LogicalVector | Boolean
Date | NumericVector| Date
POSIXct | NumericVector| DateTime
POSIXlt | NumericVector|  DateTime

We convert factors to character in R, as well as POSIXlt to POSIXct.
