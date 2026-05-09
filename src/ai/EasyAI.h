// ────────────────────────────────────────────────────────────
//  EasyAI.h
//  Placeholder for extracting Easy difficulty AI into its
//  own dedicated class when complexity warrants it.
//
//  Currently: logic lives in AIController::easyUpdate()
//  Future:    extract here and call from AIController.
// ────────────────────────────────────────────────────────────
#pragma once

// TODO: Extract easy AI strategy from AIController::easyUpdate()
// into a dedicated EasyAI class following the Strategy pattern.
//
// Suggested interface:
//
// class EasyAI {
// public:
//     void decide(Snake* cpu, const QVector<QVector<int>>& grid,
//                 int cols, int rows);
// };
