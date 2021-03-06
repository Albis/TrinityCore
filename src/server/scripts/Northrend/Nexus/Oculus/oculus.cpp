/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "oculus.h"

/*Ruby Drake,
(npc 27756) (item 37860)
(summoned by spell Ruby Essence = 37860 ---> Call Amber Drake == 49462 ---> Summon 27756)
*/
enum RubyDrake
{
    SPELL_RIDE_RUBY_DRAKE_QUE                     = 49463,          //Apply Aura: Periodic Trigger, Interval: 3 seconds ---> 49464
    SPELL_RUBY_DRAKE_SADDLE                       = 49464,          //Allows you to ride on the back of an Amber Drake. ---> Dummy
    SPELL_RUBY_SEARING_WRATH                      = 50232,          //(60 yds) - Instant - Breathes a stream of fire at an enemy dragon, dealing 6800 to 9200 Fire damage and then jumping to additional dragons within 30 yards. Each jump increases the damage by 50%. Affects up to 5 total targets
    SPELL_RUBY_EVASIVE_AURA                       = 50248,          //Instant - Allows the Ruby Drake to generate Evasive Charges when hit by hostile attacks and spells.
    SPELL_RUBY_EVASIVE_MANEUVERS                  = 50240,          //Instant - 5 sec. cooldown - Allows your drake to dodge all incoming attacks and spells. Requires Evasive Charges to use. Each attack or spell dodged while this ability is active burns one Evasive Charge. Lasts 30 sec. or until all charges are exhausted.
    //you do not have acces to until you kill Mage-Lord Urom
    SPELL_RUBY_MARTYR                             = 50253          //Instant - 10 sec. cooldown - Redirect all harmful spells cast at friendly drakes to yourself for 10 sec.
};
/*Amber Drake,
(npc 27755)  (item 37859)
(summoned by spell Amber Essence = 37859 ---> Call Amber Drake == 49461 ---> Summon 27755)
*/
enum AmberDrake
{
    SPELL_RIDE_AMBER_DRAKE_QUE                    = 49459,          //Apply Aura: Periodic Trigger, Interval: 3 seconds ---> 49460
    SPELL_AMBER_DRAKE_SADDLE                      = 49460,          //Allows you to ride on the back of an Amber Drake. ---> Dummy
    SPELL_AMBER_SHOCK_LANCE                       = 49840,         //(60 yds) - Instant - Deals 4822 to 5602 Arcane damage and detonates all Shock Charges on an enemy dragon. Damage is increased by 6525 for each detonated.
    SPELL_SHOCK_CHARGE                            = 49836,
//  SPELL_AMBER_STOP_TIME                                    //Instant - 1 min cooldown - Halts the passage of time, freezing all enemy dragons in place for 10 sec. This attack applies 5 Shock Charges to each affected target.
    //you do not have access to until you kill the  Mage-Lord Urom.
    SPELL_AMBER_TEMPORAL_RIFT                     = 49592         //(60 yds) - Channeled - Channels a temporal rift on an enemy dragon for 10 sec. While trapped in the rift, all damage done to the target is increased by 100%. In addition, for every 15, 000 damage done to a target affected by Temporal Rift, 1 Shock Charge is generated.
};

/*Emerald Drake,
(npc 27692)  (item 37815),
 (summoned by spell Emerald Essence = 37815 ---> Call Emerald Drake == 49345 ---> Summon 27692)
*/
enum EmeraldDrake
{
    SPELL_RIDE_EMERALD_DRAKE_QUE                  = 49427,         //Apply Aura: Periodic Trigger, Interval: 3 seconds ---> 49346
    SPELL_EMERALD_DRAKE_SADDLE                    = 49346,         //Allows you to ride on the back of an Amber Drake. ---> Dummy
    SPELL_EMERALD_LEECHING_POISON                 = 50328,         //(60 yds) - Instant - Poisons the enemy dragon, leeching 1300 to the caster every 2 sec. for 12 sec. Stacks up to 3 times.
    SPELL_EMERALD_TOUCH_THE_NIGHTMARE             = 50341,         //(60 yds) - Instant - Consumes 30% of the caster's max health to inflict 25, 000 nature damage to an enemy dragon and reduce the damage it deals by 25% for 30 sec.
    // you do not have access to until you kill the Mage-Lord Urom
    SPELL_EMERALD_DREAM_FUNNEL                    = 50344         //(60 yds) - Channeled - Transfers 5% of the caster's max health to a friendly drake every second for 10 seconds as long as the caster channels.
};

enum CommonDrakeSpells
{
    SPELL_DRAKE_FLAG_VISUAL                       = 53797,
    SPELL_SOAR_TRIGGER                            = 50325,
    SPELL_SOAR_BUFF                               = 50024,
    SPELL_SCALE_STATS                             = 66667,
};

enum TrashSpells
{
    // Centrifuge Construct
    SPELL_EMPOWERING_BLOWS       = 50044,
    SPELL_EMPOWERING_BLOWS_H     = 59213,

    // Ring-Lord Conjurer
    SPELL_CHARGED_SKIN           = 50717,
    SPELL_CHARGED_SKIN_H         = 59276,

    // Ring-Lord Sorceress
    SPELL_BLIZZARD               = 50175,
    SPELL_BLIZZARD_H             = 59278,
    SPELL_FLAMESTRIKE            = 16102,
    SPELL_FLAMESTRIKE_H          = 61402,
};

#define GOSSIP_ITEM_DRAKES         "So where do we go from here?"
#define GOSSIP_ITEM_BELGARISTRASZ1 "I want to fly on the wings of the Red Flight"
#define GOSSIP_ITEM_BELGARISTRASZ2 "What abilities do Ruby Drakes have?"
#define GOSSIP_ITEM_VERDISA1       "I want to fly on the wings of the Green Flight"
#define GOSSIP_ITEM_VERDISA2       "What abilities do Emerald Drakes have?"
#define GOSSIP_ITEM_ETERNOS1       "I want to fly on the wings of the Bronze Flight"
#define GOSSIP_ITEM_ETERNOS2       "What abilities do Amber Drakes have?"

#define HAS_ESSENCE(a) ((a)->HasItemCount(ITEM_EMERALD_ESSENCE, 1) || (a)->HasItemCount(ITEM_AMBER_ESSENCE, 1) || (a)->HasItemCount(ITEM_RUBY_ESSENCE, 1))

enum Drakes
{
    GOSSIP_TEXTID_DRAKES                          = 13267,
    GOSSIP_TEXTID_BELGARISTRASZ1                  = 12916,
    GOSSIP_TEXTID_BELGARISTRASZ2                  = 13466,
    GOSSIP_TEXTID_BELGARISTRASZ3                  = 13254,
    GOSSIP_TEXTID_VERDISA1                        = 1,
    GOSSIP_TEXTID_VERDISA2                        = 1,
    GOSSIP_TEXTID_VERDISA3                        = 1,
    GOSSIP_TEXTID_ETERNOS1                        = 1,
    GOSSIP_TEXTID_ETERNOS2                        = 1,
    GOSSIP_TEXTID_ETERNOS3                        = 13256,

    ITEM_EMERALD_ESSENCE                          = 37815,
    ITEM_AMBER_ESSENCE                            = 37859,
    ITEM_RUBY_ESSENCE                             = 37860,

    NPC_VERDISA                                   = 27657,
    NPC_BELGARISTRASZ                             = 27658,
    NPC_ETERNOS                                   = 27659
};

enum Says
{
    SAY_VAROS          = 0,
    SAY_UROM           = 1
};

class npc_oculus_drake : public CreatureScript
{
public:
    npc_oculus_drake() : CreatureScript("npc_oculus_drake") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (creature->GetEntry())
        {
        case NPC_VERDISA: //Verdisa
            switch (uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA1, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA2, creature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                player->AddItem(ITEM_EMERALD_ESSENCE, 1);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA3, creature->GetGUID());
                break;
            }
            break;
        case NPC_BELGARISTRASZ: //Belgaristrasz
            switch (uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ1, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ2, creature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                player->AddItem(ITEM_RUBY_ESSENCE, 1);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ3, creature->GetGUID());
                break;
            }
            break;
        case NPC_ETERNOS: //Eternos
            switch (uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS1, creature->GetGUID());
                }
                else
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS2, creature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                player->AddItem(ITEM_AMBER_ESSENCE, 1);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS3, creature->GetGUID());
                break;
            }
            break;
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (InstanceScript* instance = creature->GetInstanceScript())
        {
            if (instance->GetBossState(DATA_DRAKOS_EVENT) == DONE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRAKES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DRAKES, creature->GetGUID());
            }
        }

        return true;
    }

};

class npc_image_belgaristrasz : public CreatureScript
{
    public:
        npc_image_belgaristrasz() : CreatureScript("npc_image_belgaristrasz") { }

        struct npc_image_belgaristraszAI : public ScriptedAI
        {
            npc_image_belgaristraszAI(Creature* creature) : ScriptedAI(creature) {}

            void IsSummonedBy(Unit* summoner)
            {
                if (summoner->GetEntry() == NPC_VAROS)
                {
                Talk(SAY_VAROS);
                me->DespawnOrUnsummon(60000);
                }
                if (summoner->GetEntry() == NPC_UROM)
                {
                Talk(SAY_UROM);
                me->DespawnOrUnsummon(60000);
                }
            }            
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_image_belgaristraszAI(creature);
        }
};

class npc_oculus_drakes : public CreatureScript
{
    public:
        npc_oculus_drakes() : CreatureScript("npc_oculus_drakes") { }

        struct npc_oculus_drakesAI : public NullCreatureAI
        {
            npc_oculus_drakesAI(Creature* creature) : NullCreatureAI(creature) { }

            void PassengerBoarded(Unit* Passenger, int8 /*seat*/, bool Apply)
            {
                if (!Apply)
                    me->DespawnOrUnsummon();
                else
                {
                    me->CastSpell(me, SPELL_SCALE_STATS, true);
                    CanSoar = true;
                }
            }
            
            void EnterCombat(Unit* /*who*/)
            {
                me->RemoveAurasDueToSpell(SPELL_SOAR_TRIGGER);
                CanSoar = true;
            }
           
            void UpdateAI(const uint32 diff)
            {
                if (!me->isInCombat() && CanSoar)
                {
                    me->CastSpell(me, SPELL_SOAR_TRIGGER, true);
                    CanSoar = false;
                }
            }

        private:
            EventMap Events;
            bool CanSoar;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_oculus_drakesAI(creature);
        }
};

class npc_oculus_centrifuge_construct : public CreatureScript
{
    public:
        npc_oculus_centrifuge_construct() : CreatureScript("npc_oculus_centrifuge_construct") { }

        struct npc_oculus_centrifuge_constructAI : public ScriptedAI
        {
            npc_oculus_centrifuge_constructAI(Creature *creature) : ScriptedAI(creature) { }

            bool CanAIAttack(Unit const* target) const
            {
                return !target->IsVehicle();
            }

            void EnterCombat(Unit* who)
            {
                DoCast(DUNGEON_MODE(SPELL_EMPOWERING_BLOWS, SPELL_EMPOWERING_BLOWS_H));
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_oculus_centrifuge_constructAI(creature);
        }
};

class npc_oculus_ringlord_conjurer : public CreatureScript
{
    public:
        npc_oculus_ringlord_conjurer() : CreatureScript("npc_oculus_ringlord_conjurer") { }

        struct npc_oculus_ringlord_conjurerAI : public ScriptedAI
        {
            npc_oculus_ringlord_conjurerAI(Creature *creature) : ScriptedAI(creature) { }

            bool CanAIAttack(Unit const* target) const
            {
                return !target->IsVehicle();
            }

            void EnterCombat(Unit* who)
            {
                DoCast(DUNGEON_MODE(SPELL_CHARGED_SKIN, SPELL_CHARGED_SKIN_H));
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_oculus_ringlord_conjurerAI(creature);
        }
};

class npc_oculus_ringlord_sorceress : public CreatureScript
{
    public:
        npc_oculus_ringlord_sorceress() : CreatureScript("npc_oculus_ringlord_sorceress") { }

        struct npc_oculus_ringlord_sorceressAI : public ScriptedAI
        {
            npc_oculus_ringlord_sorceressAI(Creature *creature) : ScriptedAI(creature) { }

            bool CanAIAttack(Unit const* target) const
            {
                return !target->IsVehicle();
            }
            
            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if ((me->GetPower(POWER_MANA)/me->GetMaxPower(POWER_MANA)) *100 <= 10)
                {
                    SetCombatMovement(false);
                    DoMeleeAttackIfReady();
                }

                if ((me->GetPower(POWER_MANA)/me->GetMaxPower(POWER_MANA)) *100 >= 10)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    {
                           DoStartMovement(target, 30.0f);
                           SetCombatMovement(true);

                           DoCast(target, RAND(DUNGEON_MODE(SPELL_BLIZZARD, SPELL_BLIZZARD_H), DUNGEON_MODE(SPELL_FLAMESTRIKE, SPELL_FLAMESTRIKE_H)));
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_oculus_ringlord_sorceressAI(creature);
        }
};

class spell_oculus_touch_the_nightmare : public SpellScriptLoader
{
    public:
        spell_oculus_touch_the_nightmare() : SpellScriptLoader("spell_oculus_touch_the_nightmare") { }

        class spell_oculus_touch_the_nightmare_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_oculus_touch_the_nightmare_SpellScript);

            void OnHitEffect()
            {
                if (GetCaster())
                    GetCaster()->DealDamage(GetCaster(), (GetCaster()->GetMaxHealth()*3/10));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_oculus_touch_the_nightmare_SpellScript::OnHitEffect);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_oculus_touch_the_nightmare_SpellScript();
        }
};

class spell_oculus_dream_funnel : public SpellScriptLoader
{
    public:
        spell_oculus_dream_funnel() : SpellScriptLoader("spell_oculus_dream_funnel") { }

        class spell_oculus_dream_funnelAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_dream_funnelAuraScript);

            void HandleEffectCalcAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                // The ammount will be a 5% of the max healt, both for damage and heal
                amount = (double) (GetCaster()->GetMaxHealth()) * 0.05;
                canBeRecalculated = false;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_oculus_dream_funnelAuraScript::HandleEffectCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_oculus_dream_funnelAuraScript::HandleEffectCalcAmount, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_oculus_dream_funnelAuraScript();
        }
};

class spell_oculus_shock_lance : public SpellScriptLoader
{
    public:
        spell_oculus_shock_lance() : SpellScriptLoader("spell_oculus_shock_lance") { }

        class spell_oculus_shock_lance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_oculus_shock_lance_SpellScript);

            void RecalculateDamage()
            {
                if (Aura* charge = GetHitUnit()->GetAura(SPELL_SHOCK_CHARGE, GetHitUnit()->GetGUID()))
                {
                    SetHitDamage(6525*charge->GetStackAmount());
                    charge->Remove();
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_oculus_shock_lance_SpellScript::RecalculateDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_oculus_shock_lance_SpellScript();
        }
};

class spell_oculus_stop_time : public SpellScriptLoader
{
    public:
        spell_oculus_stop_time() : SpellScriptLoader("spell_oculus_stop_time") { }

        class spell_oculus_stop_timeAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_stop_timeAuraScript);

            void HandleOnEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();

                if (!caster || !target)
                    return;

                for (uint8 i = 0; i<5; i++)
                    caster->CastSpell(target, SPELL_SHOCK_CHARGE, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_oculus_stop_timeAuraScript::HandleOnEffectApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_oculus_stop_timeAuraScript();
        }
};

class spell_oculus_temporal_rift : public SpellScriptLoader
{
    public:
        spell_oculus_temporal_rift() : SpellScriptLoader("spell_oculus_temporal_rift") { }

        class spell_oculus_temporal_riftAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_temporal_riftAuraScript);

            uint32 previousHealth;
            uint32 acumulatedDamage;

            bool Load()
            {
                acumulatedDamage = 0;
                previousHealth = 0;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();

                if (!caster || !target)
                    return;

                uint32 charges = 0;
                uint32 damageTaken = previousHealth - target->GetHealth() + acumulatedDamage;

                if ((double) damageTaken / 15000 >= 1)
                {
                    charges = (uint32) ((double) damageTaken / 15000);
                    acumulatedDamage = damageTaken % 15000;
                }

                if (charges > 0)
                {
                    for(uint8 i = 0; i<charges; i++)
                        caster->CastSpell(target, SPELL_SHOCK_CHARGE, true);
                    previousHealth = target->GetHealth();
                    charges = 0;
                }
            }

            void HandleOnEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    previousHealth = target->GetHealth();
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_oculus_temporal_riftAuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectApply += AuraEffectApplyFn(spell_oculus_temporal_riftAuraScript::HandleOnEffectApply, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_oculus_temporal_riftAuraScript();
        }
};

void AddSC_oculus()
{
    new npc_oculus_drake();
    new npc_image_belgaristrasz();
    new npc_oculus_drakes();
    new npc_oculus_centrifuge_construct();
    new npc_oculus_ringlord_conjurer();
    new npc_oculus_ringlord_sorceress();
    new spell_oculus_touch_the_nightmare();
    new spell_oculus_dream_funnel();
    new spell_oculus_shock_lance();
    new spell_oculus_stop_time();
    new spell_oculus_temporal_rift();
}
