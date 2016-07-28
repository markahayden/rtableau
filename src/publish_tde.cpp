// rtableau: Rcpp bindings for the tableau SDK
//
// Copyright (C) 2016 Mark Hayden
//
// This file is part of rtableau.
//
// rtableau is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// rtableau is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with rtableau.  If not, see <http://www.gnu.org/licenses/>.

#include <Rcpp.h>
#include <iostream>
#include <stdlib.h>
#include "../inst/include/TableauCommon_cpp.h"
#include "../inst/include/TableauServer_cpp.h"


//' Publish a Tableau Data Extract (.tde) file to a Tableau Server
//'  using the Tableau SDK.
//'
//' @param filename a valid .tde file to publish
//' @param datasource what you want to name your extract on the server
//' @param hostname the full hostname of your Tableau Server
//' @param username Tableau Server username
//' @param password Tableau Server password
//' @param siteID The site ID, use empty string for the default site
//' @param project The name of the project on Tableau Server to publish to.
//'  If unspecified it will publish to the default project
//' @param overwrite logical. If TRUE, any existing extract with the
//'  same datasource name is destroyed. If FALSE, it will only publish
//'  if the datasource name does not already exist.
//'
//' @export
//'
//' @examples
//' publish_tde(iris, "iris.tde", "Iris", "http://localhost", "username", "password")
//'
// [[Rcpp::export]]
void publish_tde(std::wstring filename, std::wstring datasource, std::wstring hostname,
                 std::wstring username, std::wstring password, std::wstring siteID, std::wstring project, bool overwrite = false) {
  try {
    // initialize the server
    Tableau::ServerAPI::Initialize();
    Tableau::ServerConnection serverConnection;

    // connect and publish
    serverConnection.Connect(hostname, username, password, siteID);
    serverConnection.PublishExtract(filename, project, datasource, overwrite);

    // disconnect and close the virtual server
    serverConnection.Disconnect();
    serverConnection.Close();
    Tableau::ServerAPI::Cleanup();
  }
  catch (const Tableau::TableauException& e) {
    // handle errors
    std::wcerr << "Error: ";
    switch(e.GetResultCode())
    {
      case Tableau::Result::Result_InternalError:
        std::wcerr << "InternalError - Could not parse the response from the server." << std::endl;
        break;
      case Tableau::Result::Result_InvalidArgument:
        std::wcerr << "InvalidArgument - " << e.GetMessage() << std::endl;
        break;
      case Tableau::Result::Result_CurlError:
        std::wcerr << "CurlError - " << e.GetMessage() << std::endl;
        break;
      case Tableau::Result::Result_ServerError:
        std::wcerr << "ServerError - " << e.GetMessage() << std::endl;
        break;
      case Tableau::Result::Result_NotAuthenticated:
        std::wcerr << "NotAuthenticated - " << e.GetMessage() << std::endl;
        break;
      case Tableau::Result::Result_BadPayload:
        std::wcerr << "BadPayload - Unknown response from the server. Make sure this version of Tableau API is compatible with your server." << std::endl;
        break;
      case Tableau::Result::Result_InitError:
        std::wcerr << "InitError - " << e.GetMessage() << std::endl;
        break;
      case Tableau::Result::Result_UnknownError:
      default:
        std::wcerr << "An unknown error occured." << std::endl;
    }
  }
}
