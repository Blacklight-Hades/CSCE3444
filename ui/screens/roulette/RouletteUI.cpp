#include "RouletteUI.h"
#include <cmath>
#include <iomanip>
#include <sstream>

const float PI = 3.14159265358979323846f;
const float TABLE_X = 580.f;
const float TABLE_Y = 100.f;
const float BOX_W = 110.f;
const float BOX_H = 35.f;

RouletteUI::RouletteUI(sf::Font& sharedFont)
    : game(1000),
      sessionStats(nullptr),
      font(sharedFont),
      titleText(font, "ROULETTE", 54),
      bankrollText(font, "", 28),
      messageText(font, "Place your bets!", 24),
      spinText(font, "SPIN!", 24),
      clearText(font, "CLEAR", 20),
      backText(font, "BACK TO MENU", 20),
      chipSizeText(font, "Chip: $10", 24),
      infoText(font, "", 24),
      plus10Text(font, "+$10", 18),
      minus10Text(font, "-$10", 18),
      currentChipSize(10),
      betInputBoxText(font, "", 20),
      betInputString("10"),
      isTypingBet(false),
      isSpinning(false),
      spinDuration(3.0f),
      resultingNumber(-1),
      lastPayout(0)
{
    titleText.setFillColor(sf::Color(210, 120, 255));
    titleText.setPosition({ 50.f, 20.f });

    bankrollText.setFillColor(sf::Color::White);
    bankrollText.setPosition({ 50.f, 80.f });

    messageText.setFillColor(sf::Color(230, 230, 255));
    messageText.setPosition({ 50.f, 620.f });

    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition({ 50.f, 660.f });

    chipSizeText.setFillColor(sf::Color::White);
    chipSizeText.setPosition({ 50.f, 130.f });

    minus10Button.setSize({ 60.f, 40.f });
    minus10Button.setPosition({ 50.f, 170.f });
    minus10Button.setFillColor(sf::Color(180, 65, 85));

    plus10Button.setSize({ 60.f, 40.f });
    plus10Button.setPosition({ 130.f, 170.f });
    plus10Button.setFillColor(sf::Color(60, 180, 60));

    betInputBox.setSize({ 100.f, 40.f });
    betInputBox.setPosition({ 200.f, 170.f });
    betInputBox.setFillColor(sf::Color(30, 30, 60));
    betInputBox.setOutlineThickness(2.f);
    betInputBox.setOutlineColor(sf::Color(210, 120, 255));
    betInputBoxText.setFillColor(sf::Color::White);
    refreshBetInputText();

    clearButton.setSize({ 150.f, 50.f });
    clearButton.setPosition({ 580.f, 660.f });
    clearButton.setFillColor(sf::Color(180, 65, 85));

    spinButton.setSize({ 150.f, 50.f });
    spinButton.setPosition({ 760.f, 660.f });
    spinButton.setFillColor(sf::Color(125, 45, 180));

    backButton.setSize({ 180.f, 50.f });
    backButton.setPosition({ 800.f, 20.f });
    backButton.setFillColor(sf::Color(110, 110, 110));

    minus10Text.setFillColor(sf::Color::White);
    plus10Text.setFillColor(sf::Color::White);
    clearText.setFillColor(sf::Color::White);
    spinText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);

    centerTextInButton(minus10Text, minus10Button);
    centerTextInButton(plus10Text, plus10Button);
    centerTextInButton(clearText, clearButton);
    centerTextInButton(spinText, spinButton);
    centerTextInButton(backText, backButton);

    updateText();
}

void RouletteUI::setStartingBankroll(double bankroll) {
    game = RouletteGame(bankroll);
    renderedChips.clear();
    isSpinning = false;
    resultingNumber = -1;
    lastPayout = 0;
    currentChipSize = 10;
    betInputString = "10";
    isTypingBet = false;
    messageText.setString("Place your bets!");
    infoText.setString("");
    refreshBetInputText();
    updateText();
}

void RouletteUI::setSessionStats(SessionStats* stats) {
    sessionStats = stats;
}

double RouletteUI::getCurrentBankroll() const {
    return game.getBalance();
}

void RouletteUI::handleTextEntered(unsigned int unicode) {
    if (!isTypingBet) return;

    if (unicode >= '0' && unicode <= '9') {
        if (betInputString.size() < 6) {
            betInputString += static_cast<char>(unicode);
            if (!betInputString.empty()) {
                currentChipSize = std::stoi(betInputString);
            }
            refreshBetInputText();
            updateText();
        }
    }
}

void RouletteUI::handleBackspace() {
    if (!isTypingBet) return;

    if (!betInputString.empty()) {
        betInputString.pop_back();
        if (betInputString.empty()) {
            currentChipSize = 0;
        } else {
            currentChipSize = std::stoi(betInputString);
        }
        refreshBetInputText();
        updateText();
    }
}

void RouletteUI::refreshBetInputText() {
    betInputBoxText.setString(betInputString.empty() ? "$" : "$" + betInputString);
    centerTextInButton(betInputBoxText, betInputBox);
}

void RouletteUI::centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f pos = button.getPosition();
    sf::Vector2f size = button.getSize();

    text.setPosition({
        pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
        pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y
    });
}

sf::Color RouletteUI::getStandardColor(int number) const {
    if (number == 0) return sf::Color(30, 150, 60);
    int red[] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    for (int r : red) {
        if (number == r) return sf::Color(180, 45, 45);
    }
    return sf::Color(30, 30, 30);
}

void RouletteUI::updateText() {
    bankrollText.setString("Bankroll: $" + std::to_string(game.getBalance()));
    chipSizeText.setString("Chip: $" + std::to_string(currentChipSize));
    refreshBetInputText();
    
    // Disable buttons if spinning
    if (isSpinning) {
        spinButton.setFillColor(sf::Color(70, 70, 110));
        clearButton.setFillColor(sf::Color(70, 70, 110));
    } else {
        spinButton.setFillColor(renderedChips.empty() ? sf::Color(70, 70, 110) : sf::Color(125, 45, 180));
        clearButton.setFillColor(sf::Color(180, 65, 85));
    }
}

void RouletteUI::pushChip(const std::string& label, sf::Vector2f pos, RouletteBet bet) {
    if (game.canPlaceBet(currentChipSize)) {
        game.placeBet(bet);
        PlacedChip pc;
        pc.text = label;
        pc.position = pos;
        pc.amount = currentChipSize;
        pc.color = sf::Color::Yellow; // Chip color
        renderedChips.push_back(pc);
        updateText();
    }
}

void RouletteUI::handleTableClick(sf::Vector2f ms) {
    // Top 0 box
    sf::FloatRect zeroBox({TABLE_X, TABLE_Y}, {BOX_W * 3, BOX_H});
    if (zeroBox.contains(ms)) {
        pushChip("0", {TABLE_X + BOX_W * 1.5f, TABLE_Y + BOX_H / 2}, RouletteBet(BetType::StraightUp, 0, currentChipSize));
        return;
    }

    // Number grid
    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 3; ++col) {
            float x = TABLE_X + col * BOX_W;
            float y = TABLE_Y + BOX_H + row * BOX_H;
            sf::FloatRect box({x, y}, {BOX_W, BOX_H});
            if (box.contains(ms)) {
                int number = row * 3 + col + 1;
                pushChip(std::to_string(number), {x + BOX_W / 2, y + BOX_H / 2}, RouletteBet(BetType::StraightUp, number, currentChipSize));
                return;
            }
        }
    }

    // Specials boxes just below the grid: 2 boxes
    float startY = TABLE_Y + BOX_H * 13;
    float halfW = (BOX_W * 3) / 2.f;
    
    // High / Low
    if (sf::FloatRect({TABLE_X, startY}, {halfW, BOX_H}).contains(ms)) {
        pushChip("Low", {TABLE_X + halfW / 2, startY + BOX_H / 2}, RouletteBet(BetType::HighLow, 0, currentChipSize));
        return;
    }
    if (sf::FloatRect({TABLE_X + halfW, startY}, {halfW, BOX_H}).contains(ms)) {
        pushChip("High", {TABLE_X + halfW * 1.5f, startY + BOX_H / 2}, RouletteBet(BetType::HighLow, 1, currentChipSize));
        return;
    }

    // Even / Odd
    startY += BOX_H;
    if (sf::FloatRect({TABLE_X, startY}, {halfW, BOX_H}).contains(ms)) {
        pushChip("Evn", {TABLE_X + halfW / 2, startY + BOX_H / 2}, RouletteBet(BetType::EvenOdd, 0, currentChipSize));
        return;
    }
    if (sf::FloatRect({TABLE_X + halfW, startY}, {halfW, BOX_H}).contains(ms)) {
        pushChip("Odd", {TABLE_X + halfW * 1.5f, startY + BOX_H / 2}, RouletteBet(BetType::EvenOdd, 1, currentChipSize));
        return;
    }

    // Red / Black
    startY += BOX_H;
    if (sf::FloatRect({TABLE_X, startY}, {halfW, BOX_H}).contains(ms)) {
        pushChip("Red", {TABLE_X + halfW / 2, startY + BOX_H / 2}, RouletteBet(Color::Red, currentChipSize));
        return;
    }
    if (sf::FloatRect({TABLE_X + halfW, startY}, {halfW, BOX_H}).contains(ms)) {
        pushChip("Blk", {TABLE_X + halfW * 1.5f, startY + BOX_H / 2}, RouletteBet(Color::Black, currentChipSize));
        return;
    }
}

void RouletteUI::handleScreenClick(sf::Vector2f mousePos, bool& backToMenu, bool& openStats) {
    backToMenu = false;
    openStats = false;
    
    if (isSpinning) return;

    if (backButton.getGlobalBounds().contains(mousePos)) {
        backToMenu = true;
        return;
    }

    if (minus10Button.getGlobalBounds().contains(mousePos)) {
        currentChipSize = std::max(1, currentChipSize - 10);
        betInputString = std::to_string(currentChipSize);
        refreshBetInputText();
        updateText();
        return;
    }
    if (plus10Button.getGlobalBounds().contains(mousePos)) {
        currentChipSize += 10;
        betInputString = std::to_string(currentChipSize);
        refreshBetInputText();
        updateText();
        return;
    }

    if (betInputBox.getGlobalBounds().contains(mousePos)) {
        isTypingBet = true;
        betInputBox.setOutlineColor(sf::Color::Yellow);
        return;
    } else {
        isTypingBet = false;
        betInputBox.setOutlineColor(sf::Color(210, 120, 255));
    }

    if (clearButton.getGlobalBounds().contains(mousePos)) {
        game.clearBets();
        renderedChips.clear();
        updateText();
        return;
    }
    
    if (spinButton.getGlobalBounds().contains(mousePos) && !renderedChips.empty()) {
        // Compute total bet from rendered chips
        int totalBetAmount = 0;
        for (const auto& c : renderedChips) { totalBetAmount += c.amount; }

        // Reconstruct the balance before any bets were placed
        // (game.getBalance() has already had bets deducted via placeBet)
        int balanceBeforeBets = game.getBalance() + totalBetAmount;
        
        // Spin the wheel and resolve all bets
        game.spin();
        game.resolve();
        
        // NET change = final balance - balance before any betting
        int balanceAfterSpin = game.getBalance();
        lastPayout = balanceAfterSpin - balanceBeforeBets;
        
        auto res = game.getLastResult();
        resultingNumber = res.number;
        resultingColor = res.color;
        
        // Clear internal game bets (we still have renderedChips for stats)
        game.clearBets();
        
        isSpinning = true;
        spinClock.restart();
        updateText();
        
        messageText.setString("No more bets!");
        infoText.setString("");
        return;
    }

    handleTableClick(mousePos);
}

void RouletteUI::drawWheel(sf::RenderWindow& window, float elapsedTime) {
    float cx = 280.f;
    float cy = 400.f;
    float radius = 200.f;

    sf::CircleShape base(radius + 15.f);
    base.setPosition({cx - radius - 15.f, cy - radius - 15.f});
    base.setFillColor(sf::Color(80, 50, 20)); // Wooden border
    window.draw(base);

    float rotationOffset = 0.0f;
    if (isSpinning) {
        float t = std::min(elapsedTime / spinDuration, 1.0f);
        // ease out rotation
        float ease = 1.0f - std::pow(1.0f - t, 3.0f);
        rotationOffset = ease * PI * 10.0f; // Spin 5 times
    } else if (resultingNumber != -1) {
        // Find angle for the resulting number to keep it static at the top when stopped
        // For simplicity, just snap to it or leave it randomly assigned.
        // Actually, let's keep static rotation offset based on result
        rotationOffset = 10.0f * PI; // Just a static rotation
    }

    float angleStep = (2.f * PI) / 37.f;
    for (int i = 0; i < 37; ++i) {
        sf::ConvexShape slice;
        slice.setPointCount(3);
        slice.setPoint(0, {cx, cy});
        slice.setPoint(1, {cx + radius * std::cos(i * angleStep + rotationOffset), cy + radius * std::sin(i * angleStep + rotationOffset)});
        slice.setPoint(2, {cx + radius * std::cos((i + 1) * angleStep + rotationOffset), cy + radius * std::sin((i + 1) * angleStep + rotationOffset)});
        slice.setFillColor(getStandardColor(i));
        slice.setOutlineColor(sf::Color::White);
        slice.setOutlineThickness(1.f);
        window.draw(slice);

        // draw number
        sf::Text ntxt(font, std::to_string(i), 16);
        ntxt.setFillColor(sf::Color::White);
        float midAngle = (i + 0.5f) * angleStep + rotationOffset;
        ntxt.setPosition({
            cx + (radius - 25.f) * std::cos(midAngle) - 8.f,
            cy + (radius - 25.f) * std::sin(midAngle) - 8.f
        });
        window.draw(ntxt);
    }
    
    // Draw ball
    if (isSpinning || resultingNumber != -1) {
        float ballAngle = 0.f;
        float ballRad = radius - 30.f;
        if (isSpinning) {
            float t = std::min(elapsedTime / spinDuration, 1.0f);
            float easeBall = 1.0f - std::pow(1.0f - t, 2.0f);
            ballAngle = -easeBall * PI * 15.0f; // Ball spins opposite direction
            if (elapsedTime >= spinDuration - 0.5f) {
                ballRad -= (elapsedTime - (spinDuration - 0.5f)) * 40.f;
            }
        } else {
            // Find the slice of the winning number
            for (int i = 0; i < 37; ++i) {
                if (i == resultingNumber) {
                    ballAngle = (i + 0.5f) * angleStep + rotationOffset;
                    ballRad -= 20.f; // Settled in pocket
                    break;
                }
            }
        }
        
        sf::CircleShape ball(6.f);
        ball.setFillColor(sf::Color::White);
        ball.setPosition({cx + ballRad * std::cos(ballAngle) - 6.f, cy + ballRad * std::sin(ballAngle) - 6.f});
        window.draw(ball);
    }
    
    // Center pin
    sf::CircleShape pin(20.f);
    pin.setFillColor(sf::Color(190, 150, 40)); // Gold
    pin.setPosition({cx - 20.f, cy - 20.f});
    window.draw(pin);
}

void RouletteUI::drawTable(sf::RenderWindow& window) {
    // 0 box
    sf::RectangleShape zeroBox({BOX_W * 3, BOX_H});
    zeroBox.setPosition({TABLE_X, TABLE_Y});
    zeroBox.setFillColor(getStandardColor(0));
    zeroBox.setOutlineColor(sf::Color::White);
    zeroBox.setOutlineThickness(1.f);
    window.draw(zeroBox);
    sf::Text ztxt(font, "0", 20);
    ztxt.setFillColor(sf::Color::White);
    centerTextInButton(ztxt, zeroBox);
    window.draw(ztxt);

    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 3; ++col) {
            sf::RectangleShape box({BOX_W, BOX_H});
            box.setPosition({TABLE_X + col * BOX_W, TABLE_Y + BOX_H + row * BOX_H});
            int number = row * 3 + col + 1;
            box.setFillColor(getStandardColor(number));
            box.setOutlineColor(sf::Color::White);
            box.setOutlineThickness(1.f);
            window.draw(box);
            
            sf::Text ntxt(font, std::to_string(number), 20);
            ntxt.setFillColor(sf::Color::White);
            centerTextInButton(ntxt, box);
            window.draw(ntxt);
        }
    }

    float startY = TABLE_Y + BOX_H * 13;
    float halfW = (BOX_W * 3) / 2.f;

    auto drawSpecial = [&](float x, float y, float w, sf::Color c, std::string t) {
        sf::RectangleShape box({w, BOX_H});
        box.setPosition({x, y});
        box.setFillColor(c);
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(1.f);
        window.draw(box);
        sf::Text ntxt(font, t, 18);
        ntxt.setFillColor(sf::Color::White);
        centerTextInButton(ntxt, box);
        window.draw(ntxt);
    };

    drawSpecial(TABLE_X, startY, halfW, sf::Color(30, 80, 150), "1-18");
    drawSpecial(TABLE_X + halfW, startY, halfW, sf::Color(30, 80, 150), "19-36");
    startY += BOX_H;
    drawSpecial(TABLE_X, startY, halfW, sf::Color(30, 80, 150), "EVEN");
    drawSpecial(TABLE_X + halfW, startY, halfW, sf::Color(30, 80, 150), "ODD");
    startY += BOX_H;
    drawSpecial(TABLE_X, startY, halfW, sf::Color(180, 45, 45), "RED");
    drawSpecial(TABLE_X + halfW, startY, halfW, sf::Color(30, 30, 30), "BLACK");

    // Draw chips
    for (const auto& c : renderedChips) {
        sf::CircleShape chip(12.f);
        chip.setFillColor(c.color);
        chip.setOutlineColor(sf::Color::Black);
        chip.setOutlineThickness(2.f);
        chip.setPosition({c.position.x - 12.f, c.position.y - 12.f});
        window.draw(chip);

        sf::Text ctxt(font, std::to_string(c.amount), 12);
        ctxt.setFillColor(sf::Color::Black);
        ctxt.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = ctxt.getLocalBounds();
        ctxt.setPosition({
            chip.getPosition().x + (24.f - bounds.size.x) / 2.f - bounds.position.x,
            chip.getPosition().y + (24.f - bounds.size.y) / 2.f - bounds.position.y
        });
        window.draw(ctxt);
    }
}

void RouletteUI::draw(sf::RenderWindow& window) {
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(10, 40, 25));
    window.draw(background);

    float elapsed = 0.0f;
    if (isSpinning) {
        elapsed = spinClock.getElapsedTime().asSeconds();
        if (elapsed >= spinDuration) {
            isSpinning = false;
            std::string colStr = resultingColor == Color::Red ? "Red" : (resultingColor == Color::Black ? "Black" : "Green");
            
            // Compute total wagered this spin
            int totalBetAmount = 0;
            for (const auto& c : renderedChips) { totalBetAmount += c.amount; }

            // Build the result message
            messageText.setString("Ball landed on " + std::to_string(resultingNumber) + " " + colStr);
            
            if (lastPayout > 0) {
                infoText.setString("You won $" + std::to_string(lastPayout) + "!  (Wagered: $" + std::to_string(totalBetAmount) + ")");
                infoText.setFillColor(sf::Color(100, 255, 100));
            } else if (lastPayout < 0) {
                infoText.setString("You lost $" + std::to_string(-lastPayout) + ".  (Wagered: $" + std::to_string(totalBetAmount) + ")");
                infoText.setFillColor(sf::Color(255, 120, 120));
            } else {
                infoText.setString("Push — broke even.  (Wagered: $" + std::to_string(totalBetAmount) + ")");
                infoText.setFillColor(sf::Color::Yellow);
            }

            // Record to session stats
            if (sessionStats) {
                RouletteRoundSummary summary;
                summary.betAmount = totalBetAmount;
                summary.netChange = lastPayout;
                // payoutAmount = total money returned to player (0 if total loss, bet+profit if won)
                summary.payoutAmount = (lastPayout > 0) ? (totalBetAmount + lastPayout) : std::max(0, totalBetAmount + lastPayout);
                
                summary.wasStraightUp = false;
                summary.straightUpWon = false;
                for (const auto& c : renderedChips) {
                    if (c.text != "Red" && c.text != "Blk" && c.text != "Evn" && 
                        c.text != "Odd" && c.text != "Low" && c.text != "High") {
                        summary.wasStraightUp = true;
                        if (std::to_string(resultingNumber) == c.text) {
                            summary.straightUpWon = true;
                        }
                    }
                }
                sessionStats->recordRouletteRound(summary);
            }
            
            renderedChips.clear();
            updateText();
        }
    }

    drawWheel(window, elapsed);
    drawTable(window);
    
    window.draw(titleText);
    window.draw(bankrollText);
    window.draw(chipSizeText);
    window.draw(messageText);
    window.draw(infoText);
    
    window.draw(minus10Button);
    window.draw(plus10Button);
    window.draw(betInputBox);
    window.draw(minus10Text);
    window.draw(plus10Text);
    refreshBetInputText();
    window.draw(betInputBoxText);
    
    window.draw(spinButton);
    window.draw(clearButton);
    window.draw(backButton);
    
    window.draw(spinText);
    window.draw(clearText);
    window.draw(backText);
}
