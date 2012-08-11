
SET @TEROKK = 21838;
SET @TRIGGER = xxxxx; -- Creature Entry
SET @GOSSIP = 8687; -- The gameobject Got this as Data 3 (Gossip ID), And there is no such gossip, so we use that.
SET @TEXT = xxxxx; -- Text ID
SET @EQUIPENTRY = xxxx; -- Creature_Equip_Template Entry

-- Script Name for Gobject
UPDATE `gameobject_template` SET `ScriptName` = 'go_ancient_skull_pile' WHERE `entry` = 185928;

-- Remove Unused EAI And set Scriptname
DELETE FROM `creature_ai_scripts` WHERE creature_id = @TEROKK;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_terokk' WHERE `entry` = 21838;

-- Texts
DELETE FROM `creature_text` WHERE `entry` = 21838;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(21838, 0, 0, 'Who calls me to this world? The stars are not yet aligned...my powers fail me! You will pay for this!', 14, 0, 0, 0, 0, 0, 'Terokk - Summoned & inCombat'), 
(21838, 1, 0, 'Show me how tough you are!', 12, 0, 0, 0, 0, 0, 'Terokk - Player Chosen'), 
(21838, 2, 0, 'Kwa! You cannot kill me, I am immortal!', 12, 0, 0, 0, 0, 0, 'Terokk - Immune');

-- Add Trigger
DELETE FROM `creature_template` WHERE `entry` = 97016;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(@TRIGGER, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Terokk Visual Marker Trigger', '', '', 0, 80, 80, 0, 114, 114, 0, 1, 1.14286, 1, 0, 104, 138, 0, 252, 1, 2000, 0, 1, 33555200, 8, 0, 0, 0, 0, 0, 72, 106, 26, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 7, 1.35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, '', 12340);

-- Gossips for Ancient Skull Pile :
DELETE FROM `gossip_menu` WHERE `entry` = @GOSSIP;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES 
(@GOSSIP, @TEXT);

DELETE FROM `npc_text` WHERE `ID` = @TEXT;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES 
(@TEXT, 'A terrible, dark energy emanates from this pile of skulls.', '', 0, 100, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 1);

-- Weapon of Terokk :
UPDATE `creature_template` SET `equipment_id` = @EQUIPENTRY WHERE `entry` = 21838;
DELETE FROM `creature_equip_template` WHERE `entry` = @EQUIPENTRY;
INSERT INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES (@EQUIPENTRY, 19352, 0, 0);
