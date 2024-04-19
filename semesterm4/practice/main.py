#!/usr/bin/env python

#type: ignore

"""
Simple Bot to translate Telegram messages.

First, a few handler functions are defined. Then, those functions are passed to
the Application and registered at their respective places.

Then, the bot is started and runs until we press Ctrl-C on the command line.
"""
import argostranslate.package
import argostranslate.translate

import logging
import os
import sys


from telegram import ReplyKeyboardMarkup, ReplyKeyboardRemove, Update, ForceReply
from telegram.ext import (
        Application,
        CommandHandler,
        ContextTypes,
        MessageHandler,
        filters,
        ConversationHandler
    )


def divide_chunks(list, n): 
    for i in range(0, len(list), n):  
        yield list[i:i + n] 


# Language codes, populated at runtime
from_codes = []
to_codes = []
# Telegram conversation states
SELECT_FROM, SELECT_TO, READY = range(3)


# Enable logging

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

# Set higher logging level for httpx to avoid all GET and POST requests being logged
logging.getLogger("httpx").setLevel(logging.WARNING)
logger = logging.getLogger(__name__)

async def select_from(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    reply = "Вітаю! Оберіть мову, з якої будете перекладати"
    keyboard = list(divide_chunks(from_codes, 3))
    await update.message.reply_html(reply, reply_markup=ReplyKeyboardMarkup(keyboard))
    return SELECT_FROM


async def select_from_sorry(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    reply = "Оберіть мову, з якої будете перекладати"
    keyboard = list(divide_chunks(from_codes, 3))
    await update.message.reply_html(reply, reply_markup=ReplyKeyboardMarkup(keyboard))
    return SELECT_FROM


async def select_to(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    context.user_data["from"] = update.message.text

    keyboard = list(divide_chunks(to_codes, 3))
    await update.message.reply_html(
            "Оберіть мову, на яку будете перекладати",
            reply_markup=ReplyKeyboardMarkup(keyboard))
    return SELECT_TO


async def ready(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    context.user_data["to"] = update.message.text

    from_code = context.user_data["from"]
    to_code = context.user_data["to"]
    await update.message.reply_html(
            "Готовий перекладати з {} на {}!".format(from_code, to_code),
            reply_markup=ReplyKeyboardRemove())
    return READY


async def translate(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Respond to the user message."""
    if update.message is None:
        return
    if update.message.text is None:
        await update.message.reply_text("Я вмію лише перекладати текстові повідомлення.")
        return

    logger.info("Received: %s", update.message.text)
    from_code = context.user_data["from"]
    to_code = context.user_data["to"]
    # Translate
    translatedText = argostranslate.translate.translate(update.message.text, from_code, to_code)
    logger.info("Translated: %s", translatedText)
    await update.message.reply_text(translatedText, quote=True)


def main() -> None:
    """Start the bot."""
    # Download and install Argos Translate package
    logger.info("Updating Argos Translate package index...")
    argostranslate.package.update_package_index()
    available_packages = argostranslate.package.get_available_packages()
    installed_packages = argostranslate.package.get_installed_packages()

    global from_codes, to_codes

    logger.info("Installing packages...")
    for package in available_packages:
        logger.info("Package: %s", package)
        installed = False
        for installed_package in installed_packages:
            if (package.from_code == installed_package.from_code and 
                package.to_code == installed_package.to_code and
                package.package_version == installed_package.package_version):
                installed = True
                break

        from_codes.append(package.from_code)
        to_codes.append(package.to_code)

        if installed:
            logger.info("Already installed!")
        else:
            argostranslate.package.install_from_path(package.download())
    logger.info("Installed packages")

    from_codes = sorted(set(from_codes))
    to_codes = sorted(set(to_codes))

    # Create the Application and pass it your bot's token.
    token = os.environ.get("BOT_TOKEN")
    if token is None:
        logger.error("Please specify the Telegram bot token using the BOT_TOKEN environment variable.")
        sys.exit(78)

    logger.info("Token: %s", token)
    application = Application.builder().token(token).build()

    # Add conversation handler with the states FROM and TO
    conv_handler = ConversationHandler(
        entry_points=[CommandHandler("start", select_from)],
        states={
            SELECT_FROM: [
                MessageHandler(filters.Text(from_codes), select_to),
                MessageHandler(~filters.Text(from_codes), select_from_sorry)
            ],
            SELECT_TO: [
                MessageHandler(filters.Text(to_codes), ready),
                MessageHandler(~filters.Text(to_codes), select_to)
            ],
            READY: [
                MessageHandler(filters.TEXT & ~filters.COMMAND, translate)
            ]
        },
        fallbacks=[CommandHandler("start", select_from)]
    )
    application.add_handler(conv_handler)

    # Run the bot until the user presses Ctrl-C
    application.run_polling(allowed_updates=Update.ALL_TYPES)


if __name__ == "__main__":
    main()
