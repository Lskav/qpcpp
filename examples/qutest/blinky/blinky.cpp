//$file${.::blinky.cpp} ######################################################
//
// Model: blinky.qm
// File:  ${.::blinky.cpp}
//
// This code has been generated by QM 4.3.0 (https://www.state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
//$endhead${.::blinky.cpp} ###################################################
#include "qpcpp.h"  // QP/C++ framework API
#include "blinky.h" // Blinky application
#include "bsp.h"    // Board Support Package interface

//using namespace QP;

// ask QM to declare the Blinky class ----------------------------------------
//$declare${AOs::Blinky} #####################################################
//${AOs::Blinky} .............................................................
class Blinky : public QP::QActive {
private:
    QP::QTimeEvt m_timeEvt;

public:
    Blinky();

protected:
    static QP::QState initial(Blinky * const me, QP::QEvt const * const e);
    static QP::QState off(Blinky * const me, QP::QEvt const * const e);
    static QP::QState on(Blinky * const me, QP::QEvt const * const e);
};
//$enddecl${AOs::Blinky} #####################################################

// instantiate the Blinky active object --------------------------------------
static Blinky l_blinky;

// Check for the minimum required QP version
#if (QP_VERSION < 630U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.3.0 or higher required
#endif

//$define${AOs::AO_Blinky} ###################################################

// global "opaque" pointer to the Blinky AO
//${AOs::AO_Blinky} ..........................................................
QP::QActive * const AO_Blinky = &l_blinky;
//$enddef${AOs::AO_Blinky} ###################################################

// ask QM to define the Blinky class (including the state machine) -----------
//$define${AOs::Blinky} ######################################################
//${AOs::Blinky} .............................................................
//${AOs::Blinky::Blinky} .....................................................
Blinky::Blinky()
  : QActive(Q_STATE_CAST(&Blinky::initial)),
    m_timeEvt(this, TIMEOUT_SIG, 0U)
{}
//${AOs::Blinky::SM} .........................................................
QP::QState Blinky::initial(Blinky * const me, QP::QEvt const * const e) {
    //${AOs::Blinky::SM::initial}
    QS_OBJ_DICTIONARY(&l_blinky);
    QS_OBJ_DICTIONARY(&l_blinky.m_timeEvt);

    // arm the private time event to expire in 1/2s
    // and periodically every 1/2 second
    me->m_timeEvt.armX(BSP::TICKS_PER_SEC/2,
                       BSP::TICKS_PER_SEC/2);

    QS_FUN_DICTIONARY(&off);
    QS_FUN_DICTIONARY(&on);

    return Q_TRAN(&off);
}
//${AOs::Blinky::SM::off} ....................................................
QP::QState Blinky::off(Blinky * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Blinky::SM::off}
        case Q_ENTRY_SIG: {
            BSP::ledOff();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::Blinky::SM::off::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&on);
            break;
        }
        default: {
            status_ = Q_SUPER(&top);
            break;
        }
    }
    return status_;
}
//${AOs::Blinky::SM::on} .....................................................
QP::QState Blinky::on(Blinky * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Blinky::SM::on}
        case Q_ENTRY_SIG: {
            BSP::ledOn();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::Blinky::SM::on::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&off);
            break;
        }
        default: {
            status_ = Q_SUPER(&top);
            break;
        }
    }
    return status_;
}
//$enddef${AOs::Blinky} ######################################################
