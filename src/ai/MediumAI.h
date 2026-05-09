// ────────────────────────────────────────────────────────────
//  MediumAI.h
//  Placeholder for extracting Medium difficulty AI into its
//  own dedicated class when complexity warrants it.
//
//  Currently: logic lives in AIController::mediumUpdate()
//  Future:    extract here and call from AIController.
// ────────────────────────────────────────────────────────────
#pragma once

// TODO: Extract medium AI strategy from AIController::mediumUpdate()
// into a dedicated MediumAI class following the Strategy pattern.
//
// Suggested enhancements for Medium:
//   - Weighted direction scoring (goal proximity + power-up proximity)
//   - Simple dead-end detection (flood fill check before committing)
//   - Opponent-aware avoidance
//
// Suggested interface:
//
// class MediumAI {
// public:
//     void decide(Snake* cpu, const QVector<QVector<int>>& grid,
//                 const QPoint& goal,
//                 const QVector<PowerUp>& powerUps,
//                 int cols, int rows);
// };
