//
//                           SimuLTE
// Copyright (C) 2012 Antonio Virdis, Daniele Migliorini, Giovanni
// Accongiagioco, Generoso Pagano, Vincenzo Pii.
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#include "LteRealisticChannelModel.h"
#include "LteAirFrame.h"
#include "LteBinder.h"
#include "LteDeployer.h"
#include "UserTxParams.h"
#include "LteCommon.h"
#include "ExtCell.h"

#include "LtePhyBase.h"

LteRealisticChannelModel::LteRealisticChannelModel(ParameterMap& params,
    const Coord& myCoord, unsigned int band) :
    LteChannelModel(band), myCoord_(myCoord)
{
    // LOAD ALL PARAMETERS FROM XML
    // if the parameter is not explicitly reported in xml
    // a default value will be loaded
    ParameterMap::iterator it = params.find("scenario");
    if (it != params.end()) // parameter alpha has been specified in config.xml
    {
        // set nodeB height
        scenario_ = aToDeploymentScenario(it->second.stringValue());
    }
    else
        //DEFAULT
        scenario_ = URBAN_MACROCELL;
    // get nodeb-height-coefficient from config
    it = params.find("nodeb-height");
    if (it != params.end()) // parameter alpha has been specified in config.xml
    {
        // set nodeB height
        hNodeB_ = it->second.doubleValue();
        EV
           << "create Realistic Channel Model: nodeb-height set from config.xml to "
           << hNodeB_ << endl;
    }
    else
    //DEFAULT
    hNodeB_ = 25;

        // get carrierFrequency from config in GHz
    it = params.find("carrierFrequency");
    if (it != params.end()) // parameter carrierFrequency has been specified in config.xml
    {
        // set carrierFrequency
        carrierFrequency_ = it->second.doubleValue();
        EV
           << "create Realistic Channel Model: carrierFrequency set from config.xml to "
           << carrierFrequency_ << endl;
    }
    else
    //DEFUALT 2GHZ
    carrierFrequency_ = 2;

        // flag for enble/disable shadowing
    it = params.find("shadowing");
    if (it != params.end())
    {
        shadowing_ = it->second.boolValue();
        EV
           << "create Realistic Channel Model: shadowing set from config.xml to "
           << shadowing_ << endl;
    }
    else
    shadowing_ = true;

        // get UE Height from config
    it = params.find("ue-height");
    if (it != params.end()) // parameter carrierFrequency has been specified in config.xml
    {
        hUe_ = it->second.doubleValue();
        EV
           << "create Realistic Channel Model: ue Height set from config.xml to "
           << hUe_ << endl;
    }
    else
    hUe_ = 1.5;

        // get average Building Height from config
    it = params.find("building-height");
    if (it != params.end()) // parameter carrierFrequency has been specified in config.xml
    {
        hBuilding_ = it->second.doubleValue();
        EV
           << "create Realistic Channel Model: average Building Height set from config.xml to "
           << hBuilding_ << endl;
    }
    else
    hBuilding_ = 20;

        // get average Street Wide from config
    it = params.find("street-wide");
    if (it != params.end()) // parameter carrierFrequency has been specified in config.xml
    {
        wStreet_ = it->second.doubleValue();
        EV
           << "create Realistic Channel Model:average Street Wide set from config.xml to "
           << wStreet_ << endl;
    }
    else
    wStreet_ = 20;

    // get correlation distance from config
    it = params.find("correlation-distance");
    if (it != params.end()) // parameter carrierFrequency has been specified in config.xml
    {
        correlationDistance_ = it->second.doubleValue();
        EV
           << "create Realistic Channel Model: Correlation Distance from config.xml to "
           << correlationDistance_ << endl;
    }
    else
    correlationDistance_ = 50;

        //get Harq reduction
    it = params.find("harqReduction");
    if (it != params.end())
    {
        harqReduction_ = it->second.doubleValue();
    }
    else
        harqReduction_ = 0.3;

    //get lambda min threshold
    it = params.find("lambdaMinTh");
    if (it != params.end())
    {
        lambdaMinTh_ = it->second.doubleValue();
    }
    else
        lambdaMinTh_ = 0.02;

    //get lambda max threshold
    it = params.find("lambdaMaxTh");
    if (it != params.end())
    {
        lambdaMaxTh_ = it->second.doubleValue();
    }
    else
        lambdaMaxTh_ = 0.2;

    //get lambda ratio threshold
    it = params.find("lambdaRatioTh");
    if (it != params.end())
    {
        lambdaRatioTh_ = it->second.doubleValue();
    }
    else
        lambdaRatioTh_ = 20;

    //get Antenna Gain UE
    it = params.find("antennaGainUe");
    if (it != params.end())
    {
        antennaGainUe_ = it->second.doubleValue();
    }
    else
        antennaGainUe_ = 0;

    //get Antenna Gain EnB
    it = params.find("antennGainEnB");
    if (it != params.end())
    {
        antennaGainEnB_ = it->second.doubleValue();
    }
    else
        antennaGainEnB_ = 18;

    //get Antenna Gain EnB
    it = params.find("antennGainMicro");
    if (it != params.end())
    {
        antennaGainMicro_ = it->second.doubleValue();
    }
    else
        antennaGainMicro_ = 5;

    //get Thermal Noise
    it = params.find("thermalNoise");
    if (it != params.end())
    {
        thermalNoise_ = it->second.doubleValue();
    }
    else
        thermalNoise_ = -104.5;

    //get Cable Loss
    it = params.find("cable-loss");
    if (it != params.end())
    {
        cableLoss_ = it->second.doubleValue();
    }
    else
        cableLoss_ = 2;

    //get ue noise figure
    it = params.find("ue-noise-figure");
    if (it != params.end())
    {
        ueNoiseFigure_ = it->second.doubleValue();
    }
    else
        ueNoiseFigure_ = 7;

    //get bs noise figure
    it = params.find("bs-noise-figure");
    if (it != params.end())
    {
        bsNoiseFigure_ = it->second.doubleValue();
    }
    else
        bsNoiseFigure_ = 5;

    it = params.find("useTorus");
    if (it != params.end())
    {
        useTorus_ = it->second.boolValue();
    }
    else
        useTorus_ = false;

    //get flag for dynamic los,
    // if true all users will be deployed ina fixed state
    //(LOS or NLOS) and it depends on fixedLos parameter
    it = params.find("dynamic-los");
    if (it != params.end())
    {
        dynamicLos_ = it->second.boolValue();
    }
    else
        dynamicLos_ = false;

    //get flag for fixed position los/nlos
    //if true all users will be in line of sight with eNodeB
    it = params.find("fixed-los");
    if (it != params.end())
    {
        fixedLos_ = it->second.boolValue();
    }
    else
        fixedLos_ = false;

    //get flag enable/disable fading
    it = params.find("fading");
    if (it != params.end())
    {
        fading_ = it->second.boolValue();
    }
    else
        fading_ = true;

    //get fading type
    it = params.find("fading-type");
    if (it != params.end())
    {
        if (strcmp(it->second.stringValue(), "JAKES") == 0)
            fadingType_ = JAKES;
        else if (strcmp(it->second.stringValue(), "RAYLEIGH") == 0)
            fadingType_ = RAYLEIGH;
    }
    else
        fadingType_ = JAKES;

    //get number of fading paths for jakes fading
    it = params.find("fading-paths");
    if (it != params.end())
    {
        fadingPaths_ = it->second;
    }
    else
        fadingPaths_ = 6;

    // check whether the inter-cell interference is enabled or not
    it = params.find("extCell-interference");
    if (it != params.end())
    {
        enableExtCellInterference_ = it->second;
    }
    else
        enableExtCellInterference_ = false;

    it = params.find("multiCell-interference");
    if (it != params.end())
    {
        enableMultiCellInterference_ = it->second;
    }
    else
        enableMultiCellInterference_ = false;

    //get delay rms for jakes fading
    it = params.find("delay-rms");
    if (it != params.end())
    {
        delayRMS_ = it->second.doubleValue();
    }
    else
        delayRMS_ = 363e-9;
    //get binder
    binder_ = getBinder();
    //clear jakes fading map structure
    jakesFadingMap_.clear();
}

LteRealisticChannelModel::~LteRealisticChannelModel()
{
}

double LteRealisticChannelModel::getAttenuation(MacNodeId nodeId, Direction dir,
    Coord coord)
{
    double movement = .0;
    double speed = .0;

    //COMPUTE DISTANCE between ue and eNOdeB
    double sqrDistance = myCoord_.distance(coord);

    if (dir == DL) // sender is UE
        speed = computeSpeed(nodeId, myCoord_);
    else
        speed = computeSpeed(nodeId, coord);

    //If traveled distance is greater than correlation distance UE could have changed its state and
    // its visibility from eNodeb, hence it is correct to recompute the los probability
    if (movement > correlationDistance_
        || losMap_.find(nodeId) == losMap_.end())
    {
        computeLosProbability(sqrDistance, nodeId);
    }

    //compute attenuation based on selected scenario and based on LOS or NLOS
    double attenuation = 0;
    double dbp = 0;
    switch (scenario_)
    {
        case INDOOR_HOTSPOT:
            attenuation = computeIndoor(sqrDistance, nodeId);
            break;
        case URBAN_MICROCELL:
            attenuation = computeUrbanMicro(sqrDistance, nodeId);
            break;
        case URBAN_MACROCELL:
            attenuation = computeUrbanMacro(sqrDistance, nodeId);
            break;
        case RURAL_MACROCELL:
            attenuation = computeRuralMacro(sqrDistance, dbp, nodeId);
            break;
        case SUBURBAN_MACROCELL:
            attenuation = computeSubUrbanMacro(sqrDistance, dbp, nodeId);
            break;
        default:
            throw cRuntimeError("Wrong value %d for path-loss scenario", scenario_);
    }
    //    Applying shadowing only if it is enabled by configuration
    //    log-normal shadowing
    if (shadowing_)
    {
        double mean = 0;

        //Get std deviation according to los/nlos and selected scenario

        double stdDev = getStdDev(sqrDistance < dbp, nodeId);
        double time = 0;
        double space = 0;
        double att;

        // if direction is DOWNLINK it means that this module is located in UE stack than
        // the Move object associated to the UE is myMove_ varible
        // if direction is UPLINK it means that this module is located in UE stack than
        // the Move object associated to the UE is move varible

        // if shadowing for current user has never been computed
        if (lastComputedSF_.find(nodeId) == lastComputedSF_.end())
        {
            //Get the log normal shadowing with std deviation stdDev
            att = normal(mean, stdDev);

            //store the shadowing attenuation for this user and the temporal mark
            std::pair<simtime_t, double> tmp(NOW, att);
            lastComputedSF_[nodeId] = tmp;

            //If the shadowing attenuation has been computed at least one time for this user
            // and the distance traveled by the UE is greated than correlation distance
        }
        else if ((NOW - lastComputedSF_.at(nodeId).first).dbl() * speed
            > correlationDistance_)
        {

            //get the temporal mark of the last computed shadowing attenuation
            time = (NOW - lastComputedSF_.at(nodeId).first).dbl();

            //compute the traveled distance
            space = time * speed;

            //Compute shadowing with a EAW (Exponential Average Window) (step1)
            double a = exp(-0.5 * (space / correlationDistance_));

            //Get last shadowing attenuation computed
            double old = lastComputedSF_.at(nodeId).second;

            //Compute shadowing with a EAW (Exponential Average Window) (step2)
            att = a * old + sqrt(1 - pow(a, 2)) * normal(mean, stdDev);

            // Store the new computed shadowing
            std::pair<simtime_t, double> tmp(NOW, att);
            lastComputedSF_[nodeId] = tmp;

            // if the distance traveled by the UE is smaller than correlation distance shadowing attenuation remain the same
        }
        else
        {
            att = lastComputedSF_.at(nodeId).second;
        }
        attenuation += att;
    }

    // update current user position

    //if sender is a eNodeB
    if (dir == DL)
        //store the position of user
        updatePositionHistory(nodeId, myCoord_);
    else
        //sender is an UE
        updatePositionHistory(nodeId, coord);

    EV << "LteRealisticChannelModel::getAttenuation - computed attenuation at distance " << sqrDistance << " for eNb is " << attenuation << endl;

    return attenuation;
}

void LteRealisticChannelModel::updatePositionHistory(const MacNodeId nodeId,
    const Coord coord)
{
    if (positionHistory_.find(nodeId) != positionHistory_.end())
    {
        // position already updated for this TTI.
        if (positionHistory_[nodeId].back().first == NOW)
            return;
    }

    // FIXME: possible memory leak
    positionHistory_[nodeId].push(Position(NOW, coord));

    if (positionHistory_[nodeId].size() > 2) // if we have more than a past and a current element
        // drop the oldest one
        positionHistory_[nodeId].pop();
}

double LteRealisticChannelModel::computeSpeed(const MacNodeId nodeId,
    const Coord coord)
{
    double speed = 0.0;

    if (positionHistory_.find(nodeId) == positionHistory_.end())
    {
        // no entries
        return speed;
    }
    else
    {
        //compute distance traveled from last update by UE (eNodeB position is fixed)

        if (positionHistory_[nodeId].size() == 1)
        {
            //  the only element refers to present , return 0
            return speed;
        }

        double movement = positionHistory_[nodeId].front().second.distance(coord);

        if (movement <= 0.0)
            return speed;
        else
        {
            double time = (NOW.dbl()) - (positionHistory_[nodeId].front().first.dbl());
            if (time <= 0.0) // time not updated since last speed call
                throw cRuntimeError("Multiple entries detected in position history referring to same time");
            // compute speed
            speed = (movement) / (time);
        }
    }
    return speed;
}

double computeAngle(Coord center, Coord point) {
    double relx, rely, arcoSen, angle, dist;

    // compute distance between points
    dist = point.distance(center);

    // compute distance along the axis
    relx = point.x - center.x;
    rely = point.y - center.y;

    // compute the arc sine
    arcoSen = asin(rely / dist) * 180.0 / PI;

    // adjust the angle depending on the quadrants
    if (relx < 0 && rely > 0) // quadrant II
        angle = 180.0 - arcoSen;
    else if (relx < 0 && rely <= 0) // quadrant III
        angle = 180.0 - arcoSen;
    else if (relx > 0 && rely < 0) // quadrant IV
        angle = 360.0 + arcoSen;
    else
        // quadrant I
        angle = arcoSen;

//    EV << "computeAngle: angle[" << angle <<"] - arcoSen[" << arcoSen <<
//          "] - relativePos[" << relx << "," << rely <<
//          "] - siny[" << rely/dist << "] - senx[" << relx/dist <<
//          "]" << endl;

    return angle;
}

double computeAngolarAttenuation(double angle) {
    double angolarAtt;
    double angolarAttMin = 25;
    // compute attenuation due to angolar position
    // see TR 36.814 V9.0.0 for more details
    angolarAtt = 12 * pow(angle / 70.0, 2);

//  EV << "\t angolarAtt[" << angolarAtt << "]" << endl;
    // max value for angolar attenuation is 25 dB
    if (angolarAtt > angolarAttMin)
        angolarAtt = angolarAttMin;

    return angolarAtt;
}
std::vector<double> LteRealisticChannelModel::getSINR(LteAirFrame *frame, UserControlInfo* lteInfo)
{
    AttenuationVector::iterator it;
    //get tx power
    double recvPower = lteInfo->getTxPower(); // dBm

    //get move object associated to the packet
    //this object is refereed to eNodeB if direction is DL or UE if direction is UL
    Coord coord = lteInfo->getCoord();

    // position of eNb and UE
    Coord ueCoord;
    Coord enbCoord;

    double antennaGain = 0.0;
    double noiseFigure = 0.0;
    double speed = 0.0;

    // true if we are computing a CQI for the DL direction
    bool cqiDl = false;

    MacNodeId ueId = 0;
    MacNodeId eNbId = 0;

    EnbType eNbType;

    Direction dir = (Direction) lteInfo->getDirection();

    EV << "------------ GET SINR ----------------" << endl;
    //===================== PARAMETERS SETUP ============================
    /*
     * if direction is DL and this is not a feedback packet,
     * this function has been called by LteRealisticChannelModel::error() in the UE
     *
     *         DownLink error computation
     */
    if (dir == DL && (lteInfo->getFrameType() != FEEDBACKPKT))
    {
        //set noise Figure
        noiseFigure = ueNoiseFigure_; //dB
        //set antenna gain Figure
        antennaGain = antennaGainEnB_; //dB

        // get MacId for Ue and eNb
        ueId = lteInfo->getDestId();
        eNbId = lteInfo->getSourceId();

        // get position of Ue and eNb
        ueCoord = myCoord_;
        enbCoord = lteInfo->getCoord();

        speed = computeSpeed(ueId, myCoord_);
    }
    /*
     * If direction is UL OR
     * if the packet is a feedback packet
     * it means that this function is called by the feedback computation module
     *
     * located in the eNodeB that compute the feedback received by the UE
     * Hence the UE macNodeId can be taken by the sourceId of the lteInfo
     * and the speed of the UE is contained by the Move object associated to the lteinfo
     */
    else // UL/DL CQI & UL error computation
    {
        // get MacId for Ue and eNb
        ueId = lteInfo->getSourceId();
        eNbId = lteInfo->getDestId();
        eNbType = getDeployer(eNbId)->getEnbType();

        if (dir == DL)
        {
            //set noise Figure
            noiseFigure = ueNoiseFigure_; //dB
            //set antenna gain Figure
            antennaGain = antennaGainEnB_; //dB

            // use the jakes map in the UE side
            cqiDl = true;
        }
        else // if( dir == UL )
        {
            // TODO check if antennaGainEnB should be added in UL direction too
            antennaGain = antennaGainUe_;
            noiseFigure = bsNoiseFigure_;

            // use the jakes map in the eNb side
            cqiDl = false;
        }
        speed = computeSpeed(ueId, coord);

        // get position of Ue and eNb
        ueCoord = coord;
        enbCoord = myCoord_;
    }

    EV << "LteRealisticChannelModel::getSINR - srcId=" << lteInfo->getSourceId()
       << " - destId=" << lteInfo->getDestId()
       << " - DIR=" << (( dir==DL )?"DL" : "UL")
       << " - frameType=" << ((lteInfo->getFrameType()==FEEDBACKPKT)?"feedback":"other")
       << endl
       << (( getDeployer(eNbId)->getEnbType() == MACRO_ENB )? "MACRO" : "MICRO") << " - txPwr " << lteInfo->getTxPower()
       << " - ueCoord[" << ueCoord << "] - enbCoord[" << enbCoord << "] - ueId[" << ueId << "] - enbId[" << eNbId << "]" <<
    endl;
    //=================== END PARAMETERS SETUP =======================

    //=============== PATH LOSS + SHADOWING + FADING =================
    EV << "\t using parameters - noiseFigure=" << noiseFigure << " - antennaGain=" << antennaGain <<
    " - txPwr=" << lteInfo->getTxPower() << " - for ueId=" << ueId << endl;

    // attenuation for the desired signal
    double attenuation;
    if ((lteInfo->getFrameType() == FEEDBACKPKT))
        attenuation = getAttenuation(ueId, UL, coord); // dB
    else
        attenuation = getAttenuation(ueId, dir, coord); // dB

    //compute attenuation (PATHLOSS + SHADOWING)
    recvPower -= attenuation; // (dBm-dB)=dBm

    //add antenna gain
    recvPower += antennaGain; // (dBm+dB)=dBm

    //sub cable loss
    recvPower -= cableLoss_; // (dBm-dB)=dBm

    //=============== ANGOLAR ATTENUATION =================
    if (dir == DL)
    {
        //get tx angle
        LtePhyBase* ltePhy = check_and_cast<LtePhyBase*>(
                simulation.getModule(binder_->getOmnetId(eNbId))->getSubmodule(
                        "nic")->getSubmodule("phy"));

        if (ltePhy->getTxDirection() == ANISOTROPIC)
        {
            // get tx angle
            double txAngle = ltePhy->getTxAngle();

            // compute the angle between uePosition and reference axis, considering the eNb as center
            double ueAngle = computeAngle(enbCoord, ueCoord);

            // compute the reception angle between ue and eNb
            double recvAngle = fabs(txAngle - ueAngle);

            if (recvAngle > 180)
                recvAngle = 360 - recvAngle;

            // compute attenuation due to sectorial tx
            double angolarAtt = computeAngolarAttenuation(recvAngle);

            recvPower -= angolarAtt;
        }
        // else, antenna is omni-directional
    }
    //=============== END ANGOLAR ATTENUATION =================

    std::vector<double> snrVector;

    // compute and add interference due to fading
    // Apply fading for each band
    // if the phy layer is localized we can assume that for each logical band we have different fading attenuation
    // if the phy layer is distributed the number of logical band should be set to 1
    double fadingAttenuation = 0;
    //for each logical band
    for (unsigned int i = 0; i < band_; i++)
    {
        fadingAttenuation = 0;
        //if fading is enabled
        if (fading_)
        {
            //Appling fading
            if (fadingType_ == RAYLEIGH)
                fadingAttenuation = rayleighFading(ueId, i);

            else if (fadingType_ == JAKES)
                fadingAttenuation = jakesFading(ueId, speed, i, cqiDl);
        }
        // add fading contribution to the received pwr
        double finalRecvPower = recvPower + fadingAttenuation; // (dBm+dB)=dBm

        //if txmode is multi user the tx power is dived by the number of paired user
        // in db divede by 2 means -3db
        if (lteInfo->getTxMode() == MULTI_USER)
        {
            finalRecvPower -= 3;
        }

        EV << " LteRealisticChannelModel::getSINR node " << ueId
           << ((lteInfo->getFrameType() == FEEDBACKPKT) ?
            " FEEDBACK PACKET " : " NORMAL PACKET ")
           << " band " << i << " recvPower " << recvPower
           << " direction " << dirToA(dir) << " antenna gain "
           << antennaGain << " noise figure " << noiseFigure
           << " cable loss   " << cableLoss_
           << " attenuation (pathloss + shadowing) " << attenuation
           << " speed " << speed << " thermal noise " << thermalNoise_
           << " fading attenuation " << fadingAttenuation << endl;

        snrVector.push_back(finalRecvPower);
    }
    //============ END PATH LOSS + SHADOWING + FADING ===============

    /*
     * The SINR will be calculated as follows
     *
     *              Pwr
     * SINR = ---------
     *           N  +  I
     *
     * Ndb = thermalNoise_ + noiseFigure (measured in decibel)
     * I = extCellInterference + multiCellInterference
     */

    //============ MULTI CELL INTERFERENCE COMPUTATION =================
    //vector containing the sum of multiCell interference for each band
    std::vector<double> multiCellInterference; // Linear value (mW)
    // prepare data structure
    multiCellInterference.resize(band_, 0);
    if (enableMultiCellInterference_ && dir == DL)
    {
        computeMultiCellInterference(eNbId, ueId, ueCoord, (lteInfo->getFrameType() == FEEDBACKPKT), &multiCellInterference);
    }

    //============ EXTCELL INTERFERENCE COMPUTATION =================
    //vector containing the sum of multiCell interference for each band
    std::vector<double> extCellInterference; // Linear value (mW)
    // prepare data structure
    extCellInterference.resize(band_, 0);
    if (enableExtCellInterference_ && dir == DL)
    {
        computeExtCellInterference(eNbId, ueId, ueCoord, (lteInfo->getFrameType() == FEEDBACKPKT), &extCellInterference); // dBm
    }

    //===================== SINR COMPUTATION ========================
    if ((enableExtCellInterference_ || enableMultiCellInterference_) && dir == DL)
    {
        // compute and linearize total noise
        double totN = dBmToLinear(thermalNoise_ + noiseFigure);

        // denominator expressed in dBm as (N+extCell+multiCell)
        double den;
        EV << "LteRealisticChannelModel::getSINR - distance from my eNb=" << enbCoord.distance(ueCoord) << " - DIR=" << (( dir==DL )?"DL" : "UL") << endl;

        // add interference for each band
        for (unsigned int i = 0; i < band_; i++)
        {
            //               (      mW            +  mW  +        mW            )
            den = linearToDBm(extCellInterference[i] + totN + multiCellInterference[i]);

            EV << "\t ext[" << extCellInterference[i] << "] - multi[" << multiCellInterference[i] << "] - recvPwr["
               << dBmToLinear(snrVector[i]) << "] - sinr[" << snrVector[i]-den << "]\n";

            // compute final SINR
            snrVector[i] -= den;
        }
    }
    // compute snr with no intercell interference
    else
    {
        for (unsigned int i = 0; i < band_; i++)
        {
            // compute final SINR
            snrVector[i] = snrVector[i] - noiseFigure - thermalNoise_;
            EV << "LteRealisticChannelModel::getSINR - distance from eNb=" << enbCoord.distance(coord) << " - DIR=" << (( dir==DL )?"DL" : "UL") << " - snr[" << snrVector[i] << "]\n";
        }
    }

            //if sender is an eNodeB
    if (dir == DL)
        //store the position of user
        updatePositionHistory(ueId, myCoord_);
    //sender is an UE
    else
        updatePositionHistory(ueId, coord);
    return snrVector;
}

std::vector<double> LteRealisticChannelModel::getSIR(LteAirFrame *frame,
    UserControlInfo* lteInfo)
{
    AttenuationVector::iterator it;
    //get tx power
    double recvPower = lteInfo->getTxPower();

    Coord coord = lteInfo->getCoord();

    Direction dir = (Direction) lteInfo->getDirection();

    MacNodeId id = 0;
    double speed = 0.0;

    //if direction is DL
    if (dir == DL && (lteInfo->getFrameType() != FEEDBACKPKT))
    {
        id = lteInfo->getDestId();
        speed = computeSpeed(id, myCoord_);
    }

    /*
     * If direction is UL OR
     * if the packet is a feedback packet
     * it means that this function is called by the feedback computation module
     * located in the eNodeB that compute the feedback received by the UE
     * Hence the UE macNodeId can be taken by the sourceId of the lteInfo
     * and the speed of the UE is contained by the Move object associated to the lteinfo     */
    else
    {
        id = lteInfo->getSourceId();
        speed = computeSpeed(id, coord);
    }

    //Apply fading for each band
    //if the phy layer is localized we can assume that for each logical band we have different fading attenuation
    //if the phy layer is distributed the number of logical band should be set to 1
    std::vector<double> snrVector;

    double fadingAttenuation = 0;
    //for each logical band
    for (unsigned int i = 0; i < band_; i++)
    {
        fadingAttenuation = 0;
        //if fading is enabled
        if (fading_)
        {
            //Applying fading
            if (fadingType_ == RAYLEIGH)
            {
                fadingAttenuation = rayleighFading(id, i);
            }
            else if (fadingType_ == JAKES)
            {
                fadingAttenuation = jakesFading(id, speed, i, dir);
            }
        }
        // add fading contribution to the final Sinr
        double finalSnr = recvPower + fadingAttenuation;

        //if txmode is multi user the tx power is dived by the number of paired user
        // using db, to divide by 2 means -3db
        if (lteInfo->getTxMode() == MULTI_USER)
            finalSnr -= 3;

        snrVector.push_back(finalSnr);
    }

    //if sender is a eNodeB
    if (dir == DL)
        //store the position of user
        updatePositionHistory(id, myCoord_);
    //sender is an UE
    else
        updatePositionHistory(id, coord);
    return snrVector;
}

double LteRealisticChannelModel::rayleighFading(MacNodeId id,
    unsigned int band)
{
    //get raylegh variable from trace file
    double temp1 = binder_->phyPisaData.getChannel(
        getDeployer(id)->getLambda(id)->channelIndex + band);
    return linearToDb(temp1);
}

double LteRealisticChannelModel::jakesFading(MacNodeId nodeId, double speed,
    unsigned int band, bool cqiDl)
{
    /**
     * NOTE: there are two different jakes map. One on the Ue side and one on the eNb side, with different values.
     *
     * eNb side => used for CQI computation and for error-probability evaluation in UL
     * UE side  => used for error-probability evaluation in DL
     *
     * the one within eNb is referred to the UL direction
     * the one within UE is referred to the DL direction
     *
     * thus the actual map should be choosen carefully (i.e. just check the cqiDL flag)
     */
    JakesFadingMap * actualJakesMap;

    if (cqiDl) // if we are computing a DL CQI we need the Jakes Map stored on the UE side
        actualJakesMap = obtainUeJakesMap(nodeId);

    else
        actualJakesMap = &jakesFadingMap_;

    //if this is the first time that we compute fading for current user
    if (actualJakesMap->find(nodeId) == actualJakesMap->end())
    {
        //clear the map
        // FIXME: possible memory leak
        (*actualJakesMap)[nodeId].clear();

        //for each band we are going to create a jakes fading
        for (unsigned int j = 0; j < band_; j++)
        {
            //clear some structure
            JakesFadingData temp;
            temp.angleOfArrival.clear();
            temp.delaySpread.clear();

            //for each fading path
            for (int i = 0; i < fadingPaths_; i++)
            {
                //get angle of arrivals
                temp.angleOfArrival.push_back(cos(uniform(0, M_PI)));

                //get delay spread
                temp.delaySpread.push_back(exponential(delayRMS_));
            }
            //store the jakes fadint for this user
            (*actualJakesMap)[nodeId].push_back(temp);
        }
    }
    // convert carrier frequency from GHz to Hz
    double f = carrierFrequency_ * 1000000000;

    //get transmission time start (TTI =1ms)
    simtime_t t = simTime().dbl() - 0.001;

    double re_h = 0;
    double im_h = 0;

    // Compute Doppler shift.
    double doppler_shift = (speed * f) / SPEED_OF_LIGHT;

    for (int i = 0; i < fadingPaths_; i++)
    {
        // Phase shift due to Doppler => t-selectivity.
        double phi_d = actualJakesMap->at(nodeId).at(band).angleOfArrival[i] * doppler_shift;

        // Phase shift due to delay spread => f-selectivity.
        double phi_i = actualJakesMap->at(nodeId).at(band).delaySpread[i].dbl() * f;

        // Calculate resulting phase due to t-selective and f-selective fading.
        double phi = 2.00 * M_PI * (phi_d * t.dbl() - phi_i);

        // One ring model/Clarke's model plus f-selectivity according to Cavers:
        // Due to isotropic antenna gain pattern on all paths only a^2 can be received on all paths.
        // Since we are interested in attenuation a:=1, attenuation per path is then:
        double attenuation = (1.00 / sqrt(static_cast<double>(fadingPaths_)));

        // Convert to cartesian form and aggregate {Re, Im} over all fading paths.
        re_h = re_h + attenuation * cos(phi);
        im_h = im_h - attenuation * sin(phi);

//        EV << "ID=" << nodeId << " - t[" << t << "] - dopplerShift[" << doppler_shift << "] - phiD[" <<
//                phi_d << "] - phiI[" << phi_i << "] - phi[" << phi << "] - attenuation[" << attenuation << "] - f["
//                << f << "] - Band[" << band << "] - cos(phi)["
//                << cos(phi) << "]" << endl;
    }

    // Output: |H_f|^2 = absolute channel impulse response due to fading.
    // Note that this may be >1 due to constructive interference.
    return linearToDb(re_h * re_h + im_h * im_h);
}

bool LteRealisticChannelModel::error(LteAirFrame *frame,
    UserControlInfo* lteInfo)
{
    EV << "LteRealisticChannelModel::error" << endl;

    //get codeword
    unsigned char cw = lteInfo->getCw();
    //get number of codeword
    int size = lteInfo->getUserTxParams()->readCqiVector().size();

    //get position associated to the packet
    Coord coord = lteInfo->getCoord();

    //if total number of codeword is equal to 1 the cw index should be only 0
    if (size == 1)
    cw = 0;

    //get cqi used to transmit this cw
    Cqi cqi = lteInfo->getUserTxParams()->readCqiVector()[cw];

    MacNodeId id;
    Direction dir = (Direction) lteInfo->getDirection();

    //Get MacNodeId of UE
    if (dir == DL)
    id = lteInfo->getDestId();
    else
    id = lteInfo->getSourceId();

    // Get Number of RTX
    unsigned char nTx = lteInfo->getTxNumber();

    //consistency check
    if (nTx == 0)
    throw cRuntimeError("Transmissions counter should not be 0");

    //Get txmode
    TxMode txmode = (TxMode) lteInfo->getTxMode();

    // If rank is 1 and we used SMUX to transmit we have to corrupt this packet
    if (txmode == CL_SPATIAL_MULTIPLEXING
        || txmode == OL_SPATIAL_MULTIPLEXING)
    {
        //compare lambda min (smaller eingenvalues of channel matrix) with the threshold used to compute the rank
        if (binder_->phyPisaData.getLambda(id, 1) < lambdaMinTh_)
        return false;
    }

    // Take sinr
    std::vector<double> snrV = getSINR(frame, lteInfo);

    //Get the resource Block id used to transmist this packet
    RbMap rbmap = lteInfo->getGrantedBlocks();

    //Get txmode
    unsigned int itxmode = txModeToIndex[txmode];

    double bler = 0;
    std::vector<double> totalbler;
    double finalSuccess = 1;
    RbMap::iterator it;
    std::map<Band, unsigned int>::iterator jt;

    //for each Remote unit used to transmit the packet
    for (it = rbmap.begin(); it != rbmap.end(); ++it)
    {
        //for each logical band used to transmit the packet
        for (jt = it->second.begin(); jt != it->second.end(); ++jt)
        {
            //this Rb is not allocated
            if (jt->second == 0)
            continue;

            //check the antenna used in Das
            if ((lteInfo->getTxMode() == CL_SPATIAL_MULTIPLEXING
                    || lteInfo->getTxMode() == OL_SPATIAL_MULTIPLEXING)
                && rbmap.size() > 1)
            //we consider only the snr associated to the LB used
            if (it->first != lteInfo->getCw())
            continue;

            //Get the Bler
            if (cqi == 0 || cqi > 15)
            	//throw cRuntimeError("A packet has been transmitted with a cqi equal to 0 or greater than 15 cqi:%d txmode:%d dir:%d rb:%d cw:%d rtx:%d", cqi,lteInfo->getTxMode(),dir,jt->second,cw,nTx);
            	return false;
	    int snr = snrV[jt->first];//XXX because jt->first is a Band (=unsigned short)
            if (snr < 0)
            return false;
            else if (snr > binder_->phyPisaData.maxSnr())
            bler = 0;
            else
            bler = binder_->phyPisaData.getBler(itxmode, cqi - 1, snr);

            EV << "\t bler computation: [itxMode=" << itxmode << "] - [cqi-1=" << cqi-1
               << "] - [snr=" << snr << "]" << endl;

            double success = 1 - bler;
            //compute the success probability according to the number of RB used
            double successPacket = pow(success, (double)jt->second);
            // compute the success probability according to the number of LB used
            finalSuccess *= successPacket;

            EV << " LteRealisticChannelModel::error direction " << dirToA(dir)
               << " node " << id << " remote unit " << dasToA((*it).first)
               << " Band " << (*jt).first << " SNR " << snr << " CQI " << cqi
               << " BLER " << bler << " success probability " << successPacket
               << " total success probability " << finalSuccess << endl;
        }
    }
    //Compute total error probability
    double per = 1 - finalSuccess;
    //Harq Reduction
    double totalPer = per * pow(harqReduction_, nTx - 1);

    double er = uniform(0.0, 1.0);

    EV << " LteRealisticChannelModel::error direction " << dirToA(dir)
       << " node " << id << " total ERROR probability  " << per
       << " per with H-ARQ error reduction " << totalPer
       << " - CQI[" << cqi << "]- random error extracted[" << er << "]" << endl;

    if (er <= totalPer)
    {
        EV << "This is NOT your lucky day (" << er << " < " << totalPer
           << ") -> do not receive." << endl;
        // Signal too weak, we can't receive it
        return false;
    }
    // Signal is strong enough, receive this Signal
    EV << "This is your lucky day (" << er << " > " << totalPer
       << ") -> Receive AirFrame." << endl;
    return true;
}

void LteRealisticChannelModel::computeLosProbability(double d,
    MacNodeId nodeId)
{
    double p = 0;
    if (!dynamicLos_)
    {
        losMap_[nodeId] = fixedLos_;
        return;
    }
    switch (scenario_)
    {
        case INDOOR_HOTSPOT:
            if (d < 18)
                p = 1;
            else if (d >= 37)
                p = 0.5;
            else
                p = exp((-1) * ((d - 18) / 27));
            break;
        case URBAN_MICROCELL:
            p = (((18 / d) > 1) ? 1 : 18 / d) * (1 - exp(-1 * d / 36))
                + exp(-1 * d / 36);
            break;
        case URBAN_MACROCELL:
            p = (((18 / d) > 1) ? 1 : 18 / d) * (1 - exp(-1 * d / 36))
                + exp(-1 * d / 36);
            break;
        case RURAL_MACROCELL:
            if (d <= 10)
                p = 1;
            else
                p = exp(-1 * (d - 10) / 200);
            break;
        case SUBURBAN_MACROCELL:
            if (d <= 10)
                p = 1;
            else
                p = exp(-1 * (d - 10) / 1000);
            break;
        default:
            throw cRuntimeError("Wrong path-loss scenario value %d", scenario_);
    }
    double random = uniform(0.0, 1.0);
    if (random <= p)
        losMap_[nodeId] = true;
    else
        losMap_[nodeId] = false;
}

double LteRealisticChannelModel::computeIndoor(double d, MacNodeId nodeId)
{
    double a, b;
    if (losMap_[nodeId])
    {
        /*if (d > 150 || d < 3)
            throw cRuntimeError("Error LOS indoor path loss model is valid for 3<d<150");*/
        a = 16.9;
        b = 32.8;
    }
    else
    {
       /* if (d > 250 || d < 6)
            throw cRuntimeError("Error NLOS indoor path loss model is valid for 6<d<250");*/
        a = 43.3;
        b = 11.5;
    }
    return a * log10(d) + b + 20 * log10(carrierFrequency_);
}

double LteRealisticChannelModel::computeUrbanMicro(double d, MacNodeId nodeId)
{
    if (d < 10)
        d = 10;
    if (d > 5000)
        d = 5000;
    double dbp = 4 * (hNodeB_ - 1) * (hUe_ - 1)
        * ((carrierFrequency_ * 1000000000) / SPEED_OF_LIGHT);
    if (losMap_[nodeId])
    {
        /*if (d > 5000)
            throw cRuntimeError("Error LOS urban microcell path loss model is valid for d<5000 m");*/
        if (d < dbp)
            return 22 * log10(d) + 28 + 20 * log10(carrierFrequency_);
        else
            return 40 * log10(d) + 7.8 - 18 * log10(hNodeB_ - 1)
                - 18 * log10(hUe_ - 1) + 2 * log10(carrierFrequency_);
    }
    /*if (d > 2000 || d < 10)
        throw cRuntimeError("Error NLOS urban microcell path loss model is valid for 10d<2000 m");*/
    return 36.7 * log10(d) + 22.7 + 26 * log10(carrierFrequency_);
}

double LteRealisticChannelModel::computeUrbanMacro(double d, MacNodeId nodeId)
{
    if (d < 10)
        d = 10;
    if (d > 5000)
        d = 5000;
    double dbp = 4 * (hNodeB_ - 1) * (hUe_ - 1)
        * ((carrierFrequency_ * 1000000000) / SPEED_OF_LIGHT);
    if (losMap_[nodeId])
    {
        if (d > 5000)
            throw cRuntimeError("Error LOS urban microcell path loss model is valid for d<5000 m");
        if (d < dbp)
            return 22 * log10(d) + 28 + 20 * log10(carrierFrequency_);
        else
            return 40 * log10(d) + 7.8 - 18 * log10(hNodeB_ - 1)
                - 18 * log10(hUe_ - 1) + 2 * log10(carrierFrequency_);
    }
    if (d > 5000)
        throw cRuntimeError("Error NLOS urban microcell path loss model is valid for 10d<2000 m");
    double att = 161.04 - 7.1 * log10(wStreet_) + 7.5 * log10(hBuilding_)
        - (24.37 - 3.7 * pow(hBuilding_ / hNodeB_, 2)) * log10(hNodeB_)
        + (43.42 - 3.1 * log10(hNodeB_)) * (log10(d) - 3)
        + 20 * log10(carrierFrequency_)
        - (3.2 * (pow(log10(11.75 * hUe_), 2)) - 4.97);
    return att;
}

double LteRealisticChannelModel::computeSubUrbanMacro(double d, double& dbp,
    MacNodeId nodeId)
{
    if (d < 10)
        d = 10;
    if (d > 5000)
        d = 5000;
    dbp = 4 * (hNodeB_ - 1) * (hUe_ - 1)
        * ((carrierFrequency_ * 1000000000) / SPEED_OF_LIGHT);
    if (losMap_[nodeId])
    {
        if (d > 5000)
            throw cRuntimeError("Error LOS urban microcell path loss model is valid for d<5000 m");
        double a1 = (0.03 * pow(hBuilding_, 1.72));
        double b1 = 0.044 * pow(hBuilding_, 1.72);
        double a = (a1 < 10) ? a1 : 10;
        double b = (b1 < 14.72) ? b1 : 14.72;
        if (d < dbp)
        {
            double primo = 20 * log10((40 * M_PI * d * carrierFrequency_) / 3);
            double secondo = a * log10(d);
            double quarto = 0.002 * log10(hBuilding_) * d;
            return primo + secondo - b + quarto;
        }
        else
            return 20 * log10((40 * M_PI * dbp * carrierFrequency_) / 3)
                + a * log10(dbp) - b + 0.002 * log10(hBuilding_) * dbp
                + 40 * log10(d / dbp);
    }
    if (d > 5000)
        throw cRuntimeError("Error NLOS urban microcell path loss model is valid for 10d<2000 m");
    double att = 161.04 - 7.1 * log10(wStreet_) + 7.5 * log10(hBuilding_)
        - (24.37 - 3.7 * pow(hBuilding_ / hNodeB_, 2)) * log10(hNodeB_)
        + (43.42 - 3.1 * log10(hNodeB_)) * (log10(d) - 3)
        + 20 * log10(carrierFrequency_)
        - (3.2 * (pow(log10(11.75 * hUe_), 2)) - 4.97);
    return att;
}

double LteRealisticChannelModel::computeRuralMacro(double d, double& dbp,
    MacNodeId nodeId)
{
    if (d < 10)
        d = 10;
    if (d > 5000)
        d = 5000;

    dbp = 4 * (hNodeB_ - 1) * (hUe_ - 1)
        * ((carrierFrequency_ * 1000000000) / SPEED_OF_LIGHT);
    if (losMap_[nodeId])
    {
        /*if (d > 5000)
            throw cRuntimeError("Error LOS urban microcell path loss model is valid for d<5000 m");*/
        double a1 = (0.03 * pow(hBuilding_, 1.72));
        double b1 = 0.044 * pow(hBuilding_, 1.72);
        double a = (a1 < 10) ? a1 : 10;
        double b = (b1 < 14.72) ? b1 : 14.72;
        if (d < dbp)
            return 20 * log10((40 * M_PI * d * carrierFrequency_) / 3)
                + a * log10(d) - b + 0.002 * log10(hBuilding_) * d;
        else
            return 20 * log10((40 * M_PI * dbp * carrierFrequency_) / 3)
                + a * log10(dbp) - b + 0.002 * log10(hBuilding_) * dbp
                + 40 * log10(d / dbp);
    }
    /*if (d > 10000)
        throw cRuntimeError("Error NLOS urban microcell path loss model is valid for 10d<2000 m");*/
    double att = 161.04 - 7.1 * log10(wStreet_) + 7.5 * log10(hBuilding_)
        - (24.37 - 3.7 * pow(hBuilding_ / hNodeB_, 2)) * log10(hNodeB_)
        + (43.42 - 3.1 * log10(hNodeB_)) * (log10(d) - 3)
        + 20 * log10(carrierFrequency_)
        - (3.2 * (pow(log10(11.75 * hUe_), 2)) - 4.97);
    return att;
}

double LteRealisticChannelModel::getStdDev(bool dist, MacNodeId nodeId)
{
    switch (scenario_)
    {
        case URBAN_MICROCELL:
            case INDOOR_HOTSPOT:
            if (losMap_[nodeId])
                return 3.;
            else
                return 4.;
            break;
        case URBAN_MACROCELL:
            if (losMap_[nodeId])
                return 4.;
            else
                return 6.;
            break;
        case RURAL_MACROCELL:
            case SUBURBAN_MACROCELL:
            if (losMap_[nodeId])
            {
                if (dist)
                    return 4.;
                else
                    return 6.;
            }
            else
                return 8.;
            break;
        default:
            throw cRuntimeError("Wrong path-loss scenario value %d", scenario_);
    }
    return 0.0;
}

bool LteRealisticChannelModel::computeExtCellInterference(MacNodeId eNbId, MacNodeId nodeId, Coord coord, bool isCqi,
    std::vector<double>* interference)
{
    EV << "**** Ext Cell Interference **** " << endl;

    // get external cell list
    ExtCellList list = binder_->getExtCellList();
    ExtCellList::iterator it = list.begin();

    Coord c;
    double dist, // meters
        recvPwr, // watt
        recvPwrDBm, // dBm
        att, // dBm
        angolarAtt; // dBm

    //compute distance for each cell
    while (it != list.end())
    {
        // get external cell position
        c = (*it)->getPosition();
        // computer distance between UE and the ext cell
        dist = coord.distance(c);

        EV << "\t distance between UE[" << coord.x << "," << coord.y <<
            "] and extCell[" << c.x << "," << c.y << "] is -> "
            << dist << "\t";

        // compute attenuation according to some path loss model
        att = computeExtCellPathLoss(dist, nodeId);

        //=============== ANGOLAR ATTENUATION =================
        if ((*it)->getTxDirection() == OMNI)
        {
            angolarAtt = 0;
        }
        else
        {
            // compute the angle between uePosition and reference axis, considering the eNb as center
            double ueAngle = computeAngle(c, coord);

            // compute the reception angle between ue and eNb
            double recvAngle = fabs((*it)->getTxAngle() - ueAngle);

            if (recvAngle > 180)
                recvAngle = 360 - recvAngle;

            // compute attenuation due to sectorial tx
            angolarAtt = computeAngolarAttenuation(recvAngle);
        }
        //=============== END ANGOLAR ATTENUATION =================

        // TODO do we need to use (- cableLoss_ + antennaGainEnB_) in ext cells too?
        // compute and linearize received power
        recvPwrDBm = (*it)->getTxPower() - att - angolarAtt - cableLoss_ + antennaGainEnB_;
        recvPwr = dBmToLinear(recvPwrDBm);

        // add interference in those bands where the ext cell is active
        for (unsigned int i = 0; i < band_; i++) {
            int occ;
            if (isCqi)  // check slot occupation for this TTI
            {
                occ = (*it)->getBandStatus(i);
            }
            else        // error computation. We need to check the slot occupation of the previous TTI
            {
                occ = (*it)->getPrevBandStatus(i);
            }

            // if the ext cell is active, add interference
            if (occ)
            {
                (*interference)[i] += recvPwr;
            }
        }

        it++;
    }

    return true;
}

double LteRealisticChannelModel::computeExtCellPathLoss(double dist, MacNodeId nodeId)
{
    double movement = .0;
    double speed = .0;

    speed = computeSpeed(nodeId, myCoord_);

//    EV << "LteRealisticChannelModel::computeExtCellPathLoss:" << scenario_ << "-" << shadowing_ << "\n";

    //compute attenuation based on selected scenario and based on LOS or NLOS
    double attenuation = 0;
    double dbp = 0;
    switch (scenario_)
    {
        case INDOOR_HOTSPOT:
            attenuation = computeIndoor(dist, nodeId);
            break;
        case URBAN_MICROCELL:
            attenuation = computeUrbanMicro(dist, nodeId);
            break;
        case URBAN_MACROCELL:
            attenuation = computeUrbanMacro(dist, nodeId);
            break;
        case RURAL_MACROCELL:
            attenuation = computeRuralMacro(dist, dbp, nodeId);
            break;
        case SUBURBAN_MACROCELL:
            attenuation = computeSubUrbanMacro(dist, dbp, nodeId);
            break;
        default:
            throw cRuntimeError("Wrong path-loss scenario value %d", scenario_);
    }

    //TODO Apply shadowing to each interfering extCell signal

    //    Applying shadowing only if it is enabled by configuration
    //    log-normal shadowing
    if (shadowing_)
    {
        double mean = 0;

//        Get std deviation according to los/nlos and selected scenario

//        double stdDev = getStdDev(dist < dbp, nodeId);
        double time = 0;
        double space = 0;
        double att;
//
//
//        //If the shadowing attenuation has been computed at least one time for this user
//        // and the distance traveled by the UE is greated than correlation distance
//        if ((NOW - lastComputedSF_.at(nodeId).first).dbl() * speed > correlationDistance_)
//        {
//            //get the temporal mark of the last computed shadowing attenuation
//            time = (NOW - lastComputedSF_.at(nodeId).first).dbl();
//
//            //compute the traveled distance
//            space = time * speed;
//
//            //Compute shadowing with a EAW (Exponential Average Window) (step1)
//            double a = exp(-0.5 * (space / correlationDistance_));
//
//            //Get last shadowing attenuation computed
//            double old = lastComputedSF_.at(nodeId).second;
//
//            //Compute shadowing with a EAW (Exponential Average Window) (step2)
//            att = a * old + sqrt(1 - pow(a, 2)) * normal(mean, stdDev);
//        }
//         if the distance traveled by the UE is smaller than correlation distance shadowing attenuation remain the same
//        else
        {
            att = lastComputedSF_.at(nodeId).second;
        }
        EV << "(" << att << ")";
        attenuation += att;
    }

    return attenuation;
}

LteRealisticChannelModel::JakesFadingMap * LteRealisticChannelModel::obtainUeJakesMap(MacNodeId id)
{
    // obtain a reference to UE phy
    LtePhyBase * ltePhy = check_and_cast<LtePhyBase*>(
        simulation.getModule(binder_->getOmnetId(id))->getSubmodule("nic")->getSubmodule("phy"));

        // get the associated channel and get a reference to its Jakes Map
        LteRealisticChannelModel * re = dynamic_cast<LteRealisticChannelModel *>(ltePhy->getChannelModel());
        JakesFadingMap * j = re->getJakesMap();

        return j;
    }

bool LteRealisticChannelModel::computeMultiCellInterference(MacNodeId eNbId, MacNodeId ueId, Coord coord, bool isCqi,
    std::vector<double> * interference)
{
    EV << "**** Multi Cell Interference ****" << endl;

    // reference to the mac/phy/channel of each cell
    LtePhyBase * ltePhy;

    int temp;
    double att;

    double txPwr;

    std::vector<EnbInfo*> * enbList = binder_->getEnbList();
    std::vector<EnbInfo*>::iterator it = enbList->begin(), et = enbList->end();

    while(it!=et)
    {
        MacNodeId id = (*it)->id;

        if (id == eNbId)
        {
            ++it;
            continue;
        }

        // initialize eNb data structures
        if(!(*it)->init)
        {
            // obtain a reference to enb phy and obtain tx power
            ltePhy = check_and_cast<LtePhyBase*>(simulation.getModule(binder_->getOmnetId(id))->getSubmodule("nic")->getSubmodule("phy"));
            (*it)->txPwr = ltePhy->getTxPwr();//dBm

            // get tx direction
            (*it)->txDirection = ltePhy->getTxDirection();

            // get tx angle
            (*it)->txAngle = ltePhy->getTxAngle();

            // get real Channel
            (*it)->realChan = dynamic_cast<LteRealisticChannelModel *>(ltePhy->getChannelModel());

            //get reference to mac layer
            (*it)->mac = check_and_cast<LteMacEnb*>(getMacByMacNodeId(id));

            (*it)->init = true;
        }

        // compute attenuation using data structures within the cell
        att = (*it)->realChan->getAttenuation(ueId,UL,coord);
        EV << "EnbId [" << id << "] - attenuation [" << att << "]" << endl;

        //=============== ANGOLAR ATTENUATION =================
        double angolarAtt = 0;
        if ((*it)->txDirection == ANISOTROPIC)
        {
            //get tx angle
            double txAngle = (*it)->txAngle;

            // compute the angle between uePosition and reference axis, considering the eNb as center
            double ueAngle = computeAngle((*it)->realChan->myCoord_, coord);

            // compute the reception angle between ue and eNb
            double recvAngle = fabs(txAngle - ueAngle);
            if (recvAngle > 180)
                recvAngle = 360 - recvAngle;

            // compute attenuation due to sectorial tx
            angolarAtt = computeAngolarAttenuation(recvAngle);

        }
        // else, antenna is omni-directional
        //=============== END ANGOLAR ATTENUATION =================

        txPwr = (*it)->txPwr - angolarAtt - cableLoss_ + antennaGainEnB_;

        if(isCqi)// check slot occupation for this TTI
        {
            for(unsigned int i=0;i<band_;i++)
            {
                // compute the number of occupied slot (unnecessary)
                temp = (*it)->mac->getBandStatus(i);
                if(temp!=0)
                    (*interference)[i] += dBmToLinear(txPwr-att);//(dBm-dB)=dBm

                EV << "\t band " << i << " occupied " << temp << "/pwr[" << txPwr << "]-int[" << (*interference)[i] << "]" << endl;
            }
        }
        else // error computation. We need to check the slot occupation of the previous TTI
        {
            for(unsigned int i=0;i<band_;i++)
            {
                // compute the number of occupied slot (unnecessary)
                temp = (*it)->mac->getPrevBandStatus(i);
                if(temp!=0)
                    (*interference)[i] += dBmToLinear(txPwr-att);//(dBm-dB)=dBm

                EV << "\t band " << i << " occupied " << temp << "/pwr[" << txPwr << "]-int[" << (*interference)[i] << "]" << endl;
            }
        }
        ++it;
    }

    return true;
}
