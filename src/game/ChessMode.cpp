
#include <iostream>
#include <string>
#include <vector>

#include <chess.hpp>

// Run a simple terminal chess loop using Disservin/chess-library.
// Usage:
// - Enter moves in UCI (e.g., e2e4, g1f3, e7e8q) or SAN (e.g., e4, Nf3, O-O, exd5, e8=Q).
// - Commands:
//   - "fen <FEN>" to set a position
//   - "undo" to undo the last move
//   - "quit" to exit
void RunChessConsole()
{
	// Initialize start position
	chess::Board board;
	std::vector<chess::Move> history;

	auto print_board = [&]() {
		std::cout << board;
		std::cout << "FEN: " << board.getFen() << std::endl;
	};

	std::cout << "Simple Chess Console (UCI or SAN). Type 'quit' to exit." << std::endl;
	std::cout << "Examples: e2e4, Nf3, O-O, e7e8q, fen rnbqkbnr/pp1ppppp/8/2p5/8/8/PPPPPPPP/RNBQKBNR w KQkq c6 0 2" << std::endl;

	while (true) {
		print_board();
		std::cout << (board.sideToMove() == chess::Color::WHITE ? "White" : "Black") << " to move." << std::endl;
		std::cout << "Enter move (UCI or SAN), 'fen <FEN>', 'undo', or 'quit': " << std::flush;

		std::string line;
		if (!std::getline(std::cin, line)) {
			break;
		}

		// Trim leading spaces
		while (!line.empty() && std::isspace(static_cast<unsigned char>(line.front()))) line.erase(line.begin());
		// Trim trailing spaces
		while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back()))) line.pop_back();

		if (line.empty()) continue;

		if (line == "quit" || line == "exit") {
			std::cout << "Goodbye." << std::endl;
			break;
		}

		if (line == "undo") {
			if (!history.empty()) {
				const chess::Move last = history.back();
				history.pop_back();
				board.unmakeMove(last);
			} else {
				std::cout << "No moves to undo." << std::endl;
			}
			continue;
		}

		// Handle "fen <FEN...>"
		if (line.rfind("fen ", 0) == 0 && line.size() > 4) {
			std::string fen = line.substr(4);
			if (board.setFen(fen)) {
				history.clear();
				std::cout << "Position set." << std::endl;
			} else {
				std::cout << "Invalid FEN." << std::endl;
			}
			continue;
		}

		// Parse move: try UCI first, then SAN
		chess::Move userMove = chess::Move::NO_MOVE;

		if (chess::uci::isUciMove(line)) {
			userMove = chess::uci::uciToMove(board, line);
		} else {
			// Try SAN parsing
			try {
				userMove = chess::uci::parseSan(board, line);
			} catch (const std::exception& ex) {
				userMove = chess::Move::NO_MOVE;
			}
		}

		if (userMove == chess::Move::NO_MOVE) {
			std::cout << "Invalid move format. Use UCI (e2e4) or SAN (e4, Nf3, O-O)." << std::endl;
			continue;
		}

		// Validate move legality
		chess::Movelist legalMoves;
		chess::movegen::legalmoves(legalMoves, board);
		bool is_legal = false;
		for (const auto& m : legalMoves) {
			if (m == userMove) {
				is_legal = true;
				break;
			}
		}

		if (!is_legal) {
			std::cout << "Illegal move in the current position." << std::endl;
			continue;
		}

		// Apply the move
		board.makeMove(userMove);
		history.push_back(userMove);

		// Report move in SAN
		try {
			std::cout << "Played: " << chess::uci::moveToSan(board, history.back()) << std::endl;
		} catch (...) {
			// Fallback to UCI if SAN fails for any reason
			std::cout << "Played: " << chess::uci::moveToUci(history.back(), board.chess960()) << std::endl;
		}

		// Check game state
		const auto [reason, result] = board.isGameOver();
		if (result != chess::GameResult::NONE) {
			std::cout << "Game over: ";
			if (result == chess::GameResult::DRAW) {
				std::cout << "Draw";
			} else if (result == chess::GameResult::LOSE) {
				// The side to move before the last move lost, so the player who just moved won
				std::cout << ((board.sideToMove() == chess::Color::WHITE) ? "Black" : "White") << " wins";
			}
			std::cout << " (";
			switch (reason) {
				case chess::GameResultReason::CHECKMATE: std::cout << "checkmate"; break;
				case chess::GameResultReason::STALEMATE: std::cout << "stalemate"; break;
				case chess::GameResultReason::INSUFFICIENT_MATERIAL: std::cout << "insufficient material"; break;
				case chess::GameResultReason::FIFTY_MOVE_RULE: std::cout << "50-move rule"; break;
				case chess::GameResultReason::THREEFOLD_REPETITION: std::cout << "threefold repetition"; break;
				default: std::cout << "unknown"; break;
			}
			std::cout << ")." << std::endl;
			break;
		}
	}
}


