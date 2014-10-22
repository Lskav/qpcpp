//****************************************************************************
// Model: game.qm
// File:  ./mine1.cpp
//
// This code has been generated by QM tool (see state-machine.com/qm).
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
//****************************************************************************
//${.::mine1.cpp} ............................................................
#include "qp_port.h"
#include "bsp.h"
#include "game.h"

Q_DEFINE_THIS_FILE

// encapsulated delcaration of the Mine1 HSM ---------------------------------
namespace GAME {

//${AOs::Mine1} ..............................................................
class Mine1 : public QP::QMsm {
private:
    uint8_t m_x;
    uint8_t m_y;

public:
    uint8_t m_exp_ctr;

public:
    Mine1();

protected:
    static QP::QState initial(Mine1 * const me, QP::QEvt const * const e);
    static QP::QState unused  (Mine1 * const me, QP::QEvt const * const e);
    static QP::QMState const unused_s;
    static QP::QState used  (Mine1 * const me, QP::QEvt const * const e);
    static QP::QState used_x(Mine1 * const me);
    static QP::QMState const used_s;
    static QP::QState exploding  (Mine1 * const me, QP::QEvt const * const e);
    static QP::QState exploding_e(Mine1 * const me);
    static QP::QMState const exploding_s;
    static QP::QState planted  (Mine1 * const me, QP::QEvt const * const e);
    static QP::QMState const planted_s;
};

} // namespace GAME

namespace GAME {

// local objects -------------------------------------------------------------
static Mine1 l_mine1[GAME_MINES_MAX]; // a pool of type-1 mines

//............................................................................
QP::QMsm *Mine1_getInst(uint8_t id) {
    Q_REQUIRE(id < GAME_MINES_MAX);
    return &l_mine1[id];
}

// helper function to provide the ID of this mine ............................
static inline uint8_t MINE_ID(Mine1 const * const me) {
    return static_cast<uint8_t>(me - l_mine1);
}

} // namespace GAME

// Mine1 class definition ----------------------------------------------------
namespace GAME {

//${AOs::Mine1} ..............................................................
//${AOs::Mine1::Mine1} .......................................................
Mine1::Mine1()
 : QMsm(Q_STATE_CAST(&Mine1::initial))
{}

//${AOs::Mine1::SM} ..........................................................
QP::QState Mine1::initial(Mine1 * const me, QP::QEvt const * const e) {
    static QP::QMTranActTable const tatbl_ = { // transition-action table
        &unused_s,
        {
            Q_ACTION_CAST(0)  // zero terminator
        }
    };
    // ${AOs::Mine1::SM::initial}
    static bool dict_sent = false;
    if (!dict_sent) {
        dict_sent = true;
        QS_OBJ_DICTIONARY(&l_mine1[0]); // obj. dictionaries for Mine1 pool
        QS_OBJ_DICTIONARY(&l_mine1[1]);
        QS_OBJ_DICTIONARY(&l_mine1[2]);
        QS_OBJ_DICTIONARY(&l_mine1[3]);
        QS_OBJ_DICTIONARY(&l_mine1[4]);

        QS_FUN_DICTIONARY(&Mine1::initial);// fun. dictionaries for Mine1 HSM
        QS_FUN_DICTIONARY(&Mine1::unused);
        QS_FUN_DICTIONARY(&Mine1::used);
        QS_FUN_DICTIONARY(&Mine1::planted);
        QS_FUN_DICTIONARY(&Mine1::exploding);
    }
    // local signals
    QS_SIG_DICTIONARY(MINE_PLANT_SIG,    me);
    QS_SIG_DICTIONARY(MINE_DISABLED_SIG, me);
    QS_SIG_DICTIONARY(MINE_RECYCLE_SIG,  me);
    QS_SIG_DICTIONARY(SHIP_IMG_SIG,      me);
    QS_SIG_DICTIONARY(MISSILE_IMG_SIG,   me);
    return QM_TRAN_INIT(&tatbl_);
}
//${AOs::Mine1::SM::unused} ..................................................
QP::QMState const Mine1::unused_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&unused),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine1::SM::unused}
QP::QState Mine1::unused(Mine1 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine1::SM::unused::MINE_PLANT}
        case MINE_PLANT_SIG: {
            static QP::QMTranActTable const tatbl_ = { // transition-action table
                &planted_s,
                {
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            me->m_x = Q_EVT_CAST(ObjectPosEvt)->x;
            me->m_y = Q_EVT_CAST(ObjectPosEvt)->y;
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Mine1::SM::used} ....................................................
QP::QMState const Mine1::used_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&used),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(&used_x),
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine1::SM::used}
QP::QState Mine1::used_x(Mine1 * const me) {
    // tell the Tunnel that this mine is becoming disabled
    AO_Tunnel->POST(Q_NEW(MineEvt, MINE_DISABLED_SIG, MINE_ID(me)), me);
    return QM_EXIT(&used_s);
}
// ${AOs::Mine1::SM::used}
QP::QState Mine1::used(Mine1 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine1::SM::used::MINE_RECYCLE}
        case MINE_RECYCLE_SIG: {
            static struct {
                QP::QMState const *target;
                QP::QActionHandler act[2];
            } const tatbl_ = { // transition-action table
                &unused_s,
                {
                    Q_ACTION_CAST(&used_x), // exit
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Mine1::SM::used::exploding} .........................................
QP::QMState const Mine1::exploding_s = {
    &Mine1::used_s, // superstate
    Q_STATE_CAST(&exploding),
    Q_ACTION_CAST(&exploding_e),
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine1::SM::used::exploding}
QP::QState Mine1::exploding_e(Mine1 * const me) {
    me->m_exp_ctr = 0U;
    return QM_ENTRY(&exploding_s);
}
// ${AOs::Mine1::SM::used::exploding}
QP::QState Mine1::exploding(Mine1 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine1::SM::used::exploding::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Mine1::SM::used::exploding::TIME_TICK::[stillonscreen?]}
            if ((me->m_x >= GAME_SPEED_X) && (me->m_exp_ctr < 15)) {
                ++me->m_exp_ctr;  // advance the explosion counter
                me->m_x -= GAME_SPEED_X; // move explosion by 1 step

                // tell the Game to render the current stage of Explosion
                AO_Tunnel->POST(Q_NEW(ObjectImageEvt, EXPLOSION_SIG,
                                      me->m_x + 1U, (int8_t)((int)me->m_y - 4 + 2),
                                      EXPLOSION0_BMP + (me->m_exp_ctr >> 2)),
                                me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Mine1::SM::used::exploding::TIME_TICK::[else]}
            else {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &unused_s,
                    {
                        Q_ACTION_CAST(&used_x), // exit
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Mine1::SM::used::planted} ...........................................
QP::QMState const Mine1::planted_s = {
    &Mine1::used_s, // superstate
    Q_STATE_CAST(&planted),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine1::SM::used::planted}
QP::QState Mine1::planted(Mine1 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine1::SM::used::planted::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Mine1::SM::used::planted::TIME_TICK::[me->m_x>=GAME_S~}
            if (me->m_x >= GAME_SPEED_X) {
                me->m_x -= GAME_SPEED_X; // move the mine 1 step
                // tell the Tunnel to draw the Mine
                AO_Tunnel->POST(Q_NEW(ObjectImageEvt, MINE_IMG_SIG,
                                      me->m_x, me->m_y, MINE1_BMP),
                                me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Mine1::SM::used::planted::TIME_TICK::[else]}
            else {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &unused_s,
                    {
                        Q_ACTION_CAST(&used_x), // exit
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        // ${AOs::Mine1::SM::used::planted::SHIP_IMG}
        case SHIP_IMG_SIG: {
            uint8_t x   = Q_EVT_CAST(ObjectImageEvt)->x;
            uint8_t y   = Q_EVT_CAST(ObjectImageEvt)->y;
            uint8_t bmp = Q_EVT_CAST(ObjectImageEvt)->bmp;
            // ${AOs::Mine1::SM::used::planted::SHIP_IMG::[collisionwithMI~}
            if (do_bitmaps_overlap(MINE1_BMP, me->m_x, me->m_y, bmp, x, y)) {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &unused_s,
                    {
                        Q_ACTION_CAST(&used_x), // exit
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                static MineEvt const mine1_hit(HIT_MINE_SIG, 1U);
                AO_Ship->POST(&mine1_hit, me);
                // go straight to 'disabled' and let the Ship do
                // the exploding
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        // ${AOs::Mine1::SM::used::planted::MISSILE_IMG}
        case MISSILE_IMG_SIG: {
            uint8_t x   = Q_EVT_CAST(ObjectImageEvt)->x;
            uint8_t y   = Q_EVT_CAST(ObjectImageEvt)->y;
            uint8_t bmp = Q_EVT_CAST(ObjectImageEvt)->bmp;
            // ${AOs::Mine1::SM::used::planted::MISSILE_IMG::[collisionwithMI~}
            if (do_bitmaps_overlap(MINE1_BMP, me->m_x, me->m_y, bmp, x, y)) {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &exploding_s,
                    {
                        Q_ACTION_CAST(&exploding_e), // entry
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                static ScoreEvt const mine1_destroyed(DESTROYED_MINE_SIG, 25U);
                AO_Missile->POST(&mine1_destroyed, me);
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

} // namespace GAME
