#include "chaptertype.h"

#include <boost/bimap.hpp>

#include "helper.h"

boost::bimap<ChapterType,std::string> CreateChapterTypeBimap()
{
  static boost::bimap<ChapterType,std::string> m;
  {
    static bool is_done{false};
    if (is_done) return m;
    is_done = true;
  }
  typedef boost::bimap<ChapterType,std::string>::value_type Pair;
  m.insert(Pair(ChapterType::fight,"fight"));
  m.insert(Pair(ChapterType::game_lost,"game_lost"));
  m.insert(Pair(ChapterType::game_won,"game_won"));
  m.insert(Pair(ChapterType::normal,"normal"));
  m.insert(Pair(ChapterType::pawn_shop,"pawn_shop"));
  m.insert(Pair(ChapterType::play_ball,"play_ball"));
  m.insert(Pair(ChapterType::play_dice,"play_dice"));
  m.insert(Pair(ChapterType::play_pill,"play_pill"));
  m.insert(Pair(ChapterType::shop,"shop"));
  m.insert(Pair(ChapterType::test_your_luck,"test_your_luck"));
  m.insert(Pair(ChapterType::test_your_skill,"test_your_skill"));
  return m;
}

bool IsChapterType(const std::string& item_name)
{
  const auto m = CreateChapterTypeBimap();
  return m.right.find(item_name) != m.right.end();
}

ChapterType ReadChapterType(std::stringstream& s)
{
  const std::string t{ReadString(s)};
  if (!IsChapterType(t))
  {
    std::stringstream error;
    error << "Cannot read chapter type '" << t << "'";
    throw std::runtime_error(error.str());
  }
  return ToChapterType(t);
}

ChapterType ToChapterType(const std::string& item_name)
{
  const auto m = CreateChapterTypeBimap();
  assert(IsChapterType(item_name));
  return m.right.find(item_name)->second;
}

std::string ToPrettyStr(const ChapterType item)
{
  return ToPretty(ToStr(item));
}

std::string ToStr(const ChapterType item)
{
  const auto m = CreateChapterTypeBimap();
  assert(m.left.find(item) != m.left.end());
  return m.left.find(item)->second;
}

std::ostream& operator<<(std::ostream& os, const ChapterType item)
{
  os << ToStr(item);
  return os;
}
