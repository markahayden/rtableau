#' rtableau: Rcpp bindings for the Tableau SDK
#'
#' The rtableau package provides an easy-to-use interface between R data.frame
#' objects and the Tableau SDK using Rcpp.
#'
#' \code{vignette("rtableau")}
#'
#' It has two functions:
#'
#' \link{write_tde}: Write a R data.frame to a Tableau .tde file.
#'
#' \link{publish_tde}: Publish a .tde file to a Tableau Server.
#'
#' There are a number of known issues related to the Tableau SDK
#' and how it operates with this package:
#' \itemize{
#' \item The Tableau SDK spawns other processes. This can cause havoc with GUIs
#'   such as RStudio. Therefore rtableau should only be used in console R.
#' \item You must install the C/C++/Java Tableau SDK yourself following the
#'   instructions for your platform here:
#'   \url{https://onlinehelp.tableau.com/current/api/sdk/en-us/help.htm#SDK/tableau_sdk_installing.htm}
#' \item There are some issues with the 9.3 SDK on Linux when using large datasets,
#'  I recommend using the 9.2 version of the SDK which is more stable.
#' \item The Tableau SDK does not support SSL on Linux for publishing
#' \item The configure script is a WIP. On Mac, it expects the SDK to be installed in
#'   /Library/Frameworks
#' }
#' Column types set in the .tde file are inferred based on types and
#'  classes from R. The following are supported:
#' \itemize{
#' \item character (factors are converted to character)
#' \item double
#' \item integer
#' \item logical
#' \item Date
#' \item POSIXct (POSIXlt is converted to POSIXct)
#' }
#'
#'
#' To learn more about rtableau:
#' \url{https://github.com/markahayden/rtableau}
#'
#' @docType package
#' @name rtableau
#' @useDynLib rtableau
#' @import Rcpp
NULL
