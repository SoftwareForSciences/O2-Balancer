// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_INFORMATIONNODE_INFO_SETTINGS_CONNECTION_H
#define O2_INFORMATIONNODE_INFO_SETTINGS_CONNECTION_H

#include <O2/Balancer/Utilities/Settings.h>

namespace O2{
    namespace InformationNode{

        class InfoSettings : public Balancer::Settings{
            int heartRate;
            int acknowledgePort;
            int heartBeatPort; 
            int sampleSize;
        protected:
            std::string getSettingsFile() const override;

        public:
            InfoSettings(const boost::program_options::variables_map& settings);
            int getHeartRate() const;
            int getAcknowledgePort() const;
            int getHeartBeatPort() const;
            int getSampleSize() const;
        };
    }

}

#endif